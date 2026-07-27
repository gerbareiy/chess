module;
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <cerrno>
#include <csignal>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>
#endif
#include <chrono>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>
export module Chess.E2E.ChildProcess;

namespace Chess::E2E
{
    struct ChildProcessHandles
    {
#ifdef _WIN32
        PROCESS_INFORMATION processInfo{};
        HANDLE              input  = nullptr;
        HANDLE              output = nullptr;
#else
        pid_t                        processId = -1;
        int                          input     = -1;
        int                          output    = -1;
        std::optional<unsigned long> exitCode;
#endif
    };

    // The only place in the harness that talks to the operating system: it starts a child process with its standard
    // input and output redirected into pipes, and lets the caller read, write, wait for and kill it.
    export class ChildProcess
    {
    public:
        static ChildProcess Start(
            const std::filesystem::path& exePath, const std::filesystem::path& workingDirectory, const std::vector<std::string>& arguments)
        {
#ifdef _WIN32
            SECURITY_ATTRIBUTES inheritable{};
            inheritable.nLength              = sizeof(SECURITY_ATTRIBUTES);
            inheritable.bInheritHandle       = TRUE;
            inheritable.lpSecurityDescriptor = nullptr;

            HANDLE inputRead   = nullptr;
            HANDLE inputWrite  = nullptr;
            HANDLE outputRead  = nullptr;
            HANDLE outputWrite = nullptr;

            if (!CreatePipe(&inputRead, &inputWrite, &inheritable, 0))
            {
                ThrowLastError("CreatePipe (stdin) failed");
            }
            if (!CreatePipe(&outputRead, &outputWrite, &inheritable, 0))
            {
                CloseHandle(inputRead);
                CloseHandle(inputWrite);
                ThrowLastError("CreatePipe (stdout) failed");
            }

            SetHandleInformation(inputWrite, HANDLE_FLAG_INHERIT, 0);
            SetHandleInformation(outputRead, HANDLE_FLAG_INHERIT, 0);

            STARTUPINFOW startupInfo{};
            startupInfo.cb         = sizeof(startupInfo);
            startupInfo.dwFlags    = STARTF_USESTDHANDLES;
            startupInfo.hStdInput  = inputRead;
            startupInfo.hStdOutput = outputWrite;
            startupInfo.hStdError  = outputWrite;

            std::wstring commandLine = L"\"" + exePath.wstring() + L"\"";
            for (const auto& argument : arguments)
            {
                commandLine += L" \"" + WidenAscii(argument) + L"\"";
            }

            PROCESS_INFORMATION processInfo{};
            const BOOL          started = CreateProcessW(
                nullptr,
                commandLine.data(),
                nullptr,
                nullptr,
                TRUE,
                CREATE_NO_WINDOW,
                nullptr,
                workingDirectory.empty() ? nullptr : workingDirectory.c_str(),
                &startupInfo,
                &processInfo);

            CloseHandle(inputRead);
            CloseHandle(outputWrite);

            if (!started)
            {
                CloseHandle(inputWrite);
                CloseHandle(outputRead);
                ThrowLastError("CreateProcessW failed for " + exePath.string());
            }

            return ChildProcess({ .processInfo = processInfo, .input = inputWrite, .output = outputRead });
#else
            IgnoreBrokenPipeSignal();

            int inputPipe[2]  = { -1, -1 };
            int outputPipe[2] = { -1, -1 };

            if (pipe(inputPipe) != 0)
            {
                ThrowLastError("pipe (stdin) failed");
            }
            if (pipe(outputPipe) != 0)
            {
                close(inputPipe[0]);
                close(inputPipe[1]);
                ThrowLastError("pipe (stdout) failed");
            }

            // Everything the child needs is prepared before the fork, so between fork and execv it only makes
            // async-signal-safe calls and never allocates.
            const auto executable = exePath.string();
            const auto directory  = workingDirectory.string();

            auto storage = std::vector<std::string>();
            storage.reserve(arguments.size() + 1);
            storage.push_back(executable);
            storage.insert(storage.end(), arguments.begin(), arguments.end());

            auto argumentValues = std::vector<char*>();
            argumentValues.reserve(storage.size() + 1);
            for (auto& argument : storage)
            {
                argumentValues.push_back(argument.data());
            }
            argumentValues.push_back(nullptr);

            const pid_t processId = fork();
            if (processId < 0)
            {
                close(inputPipe[0]);
                close(inputPipe[1]);
                close(outputPipe[0]);
                close(outputPipe[1]);
                ThrowLastError("fork failed for " + executable);
            }

            if (processId == 0)
            {
                if (dup2(inputPipe[0], STDIN_FILENO) < 0 || dup2(outputPipe[1], STDOUT_FILENO) < 0 || dup2(outputPipe[1], STDERR_FILENO) < 0)
                {
                    _exit(EXEC_FAILED_EXIT_CODE);
                }

                close(inputPipe[0]);
                close(inputPipe[1]);
                close(outputPipe[0]);
                close(outputPipe[1]);

                if (!directory.empty() && chdir(directory.c_str()) != 0)
                {
                    _exit(EXEC_FAILED_EXIT_CODE);
                }

                execv(executable.c_str(), argumentValues.data());
                _exit(EXEC_FAILED_EXIT_CODE);
            }

            close(inputPipe[0]);
            close(outputPipe[1]);

            return ChildProcess({ .processId = processId, .input = inputPipe[1], .output = outputPipe[0] });
#endif
        }

        ChildProcess(ChildProcess&& other) noexcept
        {
            Swap(other);
        }

        ChildProcess& operator=(ChildProcess&& other) noexcept
        {
            if (this != &other)
            {
                Release();
                Swap(other);
            }
            return *this;
        }

        ChildProcess(const ChildProcess&)            = delete;
        ChildProcess& operator=(const ChildProcess&) = delete;

        ~ChildProcess()
        {
            Release();
        }

        // Lets the child see the end of its input, which is what makes it stop waiting for the next command.
        void CloseInput()
        {
#ifdef _WIN32
            if (handles_.input != nullptr)
            {
                CloseHandle(handles_.input);
                handles_.input = nullptr;
            }
#else
            if (handles_.input >= 0)
            {
                close(handles_.input);
                handles_.input = -1;
            }
#endif
        }

        bool IsRunning()
        {
#ifdef _WIN32
            DWORD exitCode = STILL_ACTIVE;
            return GetExitCodeProcess(handles_.processInfo.hProcess, &exitCode) && exitCode == STILL_ACTIVE;
#else
            return handles_.processId > 0 && !TryReap();
#endif
        }

        void Kill()
        {
#ifdef _WIN32
            TerminateProcess(handles_.processInfo.hProcess, 1);
#else
            if (handles_.processId > 0 && !handles_.exitCode.has_value())
            {
                kill(handles_.processId, SIGKILL);
            }
#endif
        }

        // Blocks until the child writes something; zero means its output has ended for good.
        size_t Read(char* buffer, size_t size) const
        {
#ifdef _WIN32
            DWORD bytesRead = 0;
            if (!ReadFile(handles_.output, buffer, static_cast<DWORD>(size), &bytesRead, nullptr))
            {
                return 0;
            }
            return bytesRead;
#else
            while (true)
            {
                const auto bytesRead = read(handles_.output, buffer, size);
                if (bytesRead < 0 && errno == EINTR)
                {
                    continue;
                }
                return bytesRead > 0 ? static_cast<size_t>(bytesRead) : 0;
            }
#endif
        }

        std::optional<unsigned long> TryWaitForExit(std::chrono::milliseconds timeout)
        {
#ifdef _WIN32
            if (WaitForSingleObject(handles_.processInfo.hProcess, static_cast<DWORD>(timeout.count())) != WAIT_OBJECT_0)
            {
                return std::nullopt;
            }

            DWORD exitCode = 0;
            if (!GetExitCodeProcess(handles_.processInfo.hProcess, &exitCode))
            {
                return std::nullopt;
            }
            return exitCode;
#else
            const auto deadline = std::chrono::steady_clock::now() + timeout;
            while (true)
            {
                if (TryReap())
                {
                    return handles_.exitCode;
                }
                if (std::chrono::steady_clock::now() >= deadline)
                {
                    return std::nullopt;
                }
                std::this_thread::sleep_for(POLL_INTERVAL);
            }
#endif
        }

        void Write(const std::string& bytes) const
        {
#ifdef _WIN32
            DWORD written = 0;
            WriteFile(handles_.input, bytes.data(), static_cast<DWORD>(bytes.size()), &written, nullptr);
#else
            size_t offset = 0;
            while (offset < bytes.size())
            {
                const auto written = write(handles_.input, bytes.data() + offset, bytes.size() - offset);
                if (written < 0)
                {
                    if (errno == EINTR)
                    {
                        continue;
                    }
                    return;
                }
                offset += static_cast<size_t>(written);
            }
#endif
        }

    private:
#ifndef _WIN32
        static constexpr int EXEC_FAILED_EXIT_CODE = 127;
        static constexpr int SIGNALLED_EXIT_BASE   = 128;
#endif
        static constexpr std::chrono::milliseconds POLL_INTERVAL = std::chrono::milliseconds(10);

        ChildProcessHandles handles_;

        explicit ChildProcess(ChildProcessHandles handles)
            : handles_(std::move(handles))
        {
        }

        [[noreturn]] static void ThrowLastError(const std::string& what)
        {
#ifdef _WIN32
            throw std::runtime_error(what + " (GetLastError=" + std::to_string(GetLastError()) + ")");
#else
            throw std::runtime_error(what + " (errno=" + std::to_string(errno) + ": " + std::strerror(errno) + ")");
#endif
        }

#ifdef _WIN32
        static std::wstring WidenAscii(const std::string& text)
        {
            auto widened = std::wstring();
            widened.reserve(text.size());
            for (const char symbol : text)
            {
                widened.push_back(static_cast<wchar_t>(static_cast<unsigned char>(symbol)));
            }
            return widened;
        }
#else
        // Writing to a child that has already exited raises SIGPIPE, which would kill the test process outright
        // instead of just failing the write, so the signal is switched off once for the whole harness.
        static void IgnoreBrokenPipeSignal()
        {
            [[maybe_unused]] static const bool ignored = std::signal(SIGPIPE, SIG_IGN) != SIG_ERR;
        }

        static unsigned long ToExitCode(int status)
        {
            if (WIFEXITED(status))
            {
                return static_cast<unsigned long>(WEXITSTATUS(status));
            }
            if (WIFSIGNALED(status))
            {
                return static_cast<unsigned long>(SIGNALLED_EXIT_BASE + WTERMSIG(status));
            }
            return 0;
        }

        // A child can be reaped only once, so its exit code is remembered for every later question about it.
        bool TryReap()
        {
            if (handles_.exitCode.has_value())
            {
                return true;
            }

            int        status = 0;
            const auto reaped = waitpid(handles_.processId, &status, WNOHANG);
            if (reaped != handles_.processId)
            {
                return false;
            }

            handles_.exitCode = ToExitCode(status);
            return true;
        }
#endif

        void Swap(ChildProcess& other) noexcept
        {
            std::swap(handles_, other.handles_);
        }

        void Release()
        {
#ifdef _WIN32
            if (handles_.processInfo.hProcess != nullptr)
            {
                if (IsRunning())
                {
                    Kill();
                }
                CloseHandle(handles_.processInfo.hThread);
                CloseHandle(handles_.processInfo.hProcess);
            }

            CloseInput();
            if (handles_.output != nullptr)
            {
                CloseHandle(handles_.output);
            }
#else
            if (handles_.processId > 0 && !handles_.exitCode.has_value())
            {
                Kill();

                int status = 0;
                while (waitpid(handles_.processId, &status, 0) < 0 && errno == EINTR)
                {
                }
            }

            CloseInput();
            if (handles_.output >= 0)
            {
                close(handles_.output);
            }
#endif
            handles_ = {};
        }
    };
} // namespace Chess::E2E
