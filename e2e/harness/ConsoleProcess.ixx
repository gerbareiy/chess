module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <chrono>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>
export module Chess.E2E.ConsoleProcess;

namespace Chess::E2E
{
    struct ConsoleProcessState
    {
        PROCESS_INFORMATION processInfo{};
        HANDLE              stdinWrite = nullptr;
        HANDLE              stdoutRead = nullptr;
        std::thread         readerThread;
        std::mutex          bufferMutex;
        std::string         buffer;
        size_t              searchCursor = 0;

        ~ConsoleProcessState()
        {
            if (processInfo.hProcess != nullptr)
            {
                DWORD exitCode = STILL_ACTIVE;
                if (GetExitCodeProcess(processInfo.hProcess, &exitCode) && exitCode == STILL_ACTIVE)
                {
                    TerminateProcess(processInfo.hProcess, 1);
                }
            }

            if (stdinWrite != nullptr)
            {
                CloseHandle(stdinWrite);
            }
            if (readerThread.joinable())
            {
                readerThread.join();
            }
            if (stdoutRead != nullptr)
            {
                CloseHandle(stdoutRead);
            }
            if (processInfo.hThread != nullptr)
            {
                CloseHandle(processInfo.hThread);
            }
            if (processInfo.hProcess != nullptr)
            {
                CloseHandle(processInfo.hProcess);
            }
        }
    };

    export class ConsoleProcess
    {
    public:
        static ConsoleProcess Start(
            const std::filesystem::path& exePath, const std::filesystem::path& workingDirectory, const std::vector<std::string>& arguments = {})
        {
            SECURITY_ATTRIBUTES saInheritable{};
            saInheritable.nLength              = sizeof(SECURITY_ATTRIBUTES);
            saInheritable.bInheritHandle       = TRUE;
            saInheritable.lpSecurityDescriptor = nullptr;

            HANDLE stdinRead   = nullptr;
            HANDLE stdinWrite  = nullptr;
            HANDLE stdoutRead  = nullptr;
            HANDLE stdoutWrite = nullptr;

            if (!CreatePipe(&stdinRead, &stdinWrite, &saInheritable, 0))
            {
                ThrowLastError("CreatePipe (stdin) failed");
            }
            if (!CreatePipe(&stdoutRead, &stdoutWrite, &saInheritable, 0))
            {
                ThrowLastError("CreatePipe (stdout) failed");
            }

            SetHandleInformation(stdinWrite, HANDLE_FLAG_INHERIT, 0);
            SetHandleInformation(stdoutRead, HANDLE_FLAG_INHERIT, 0);

            STARTUPINFOW startupInfo{};
            startupInfo.cb         = sizeof(startupInfo);
            startupInfo.dwFlags    = STARTF_USESTDHANDLES;
            startupInfo.hStdInput  = stdinRead;
            startupInfo.hStdOutput = stdoutWrite;
            startupInfo.hStdError  = stdoutWrite;

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

            CloseHandle(stdinRead);
            CloseHandle(stdoutWrite);

            if (!started)
            {
                CloseHandle(stdinWrite);
                CloseHandle(stdoutRead);
                ThrowLastError("CreateProcessW failed for " + exePath.string());
            }

            auto state         = std::make_unique<ConsoleProcessState>();
            state->processInfo = processInfo;
            state->stdinWrite  = stdinWrite;
            state->stdoutRead  = stdoutRead;

            ConsoleProcessState* rawState = state.get();
            state->readerThread           = std::thread(
                [rawState]
                {
                    char buffer[4096];
                    while (true)
                    {
                        DWORD bytesRead = 0;
                        if (!ReadFile(rawState->stdoutRead, buffer, sizeof(buffer), &bytesRead, nullptr) || bytesRead == 0)
                        {
                            return;
                        }
                        std::lock_guard lock(rawState->bufferMutex);
                        rawState->buffer.append(buffer, bytesRead);
                    }
                });

            return ConsoleProcess(std::move(state));
        }

        ConsoleProcess(ConsoleProcess&&) noexcept            = default;
        ConsoleProcess& operator=(ConsoleProcess&&) noexcept = default;
        ConsoleProcess(const ConsoleProcess&)                = delete;
        ConsoleProcess& operator=(const ConsoleProcess&)     = delete;
        ~ConsoleProcess()                                    = default;

        void SendLine(const std::string& line) const
        {
            const std::string withNewline = line + "\n";
            DWORD             written     = 0;
            WriteFile(state_->stdinWrite, withNewline.data(), static_cast<DWORD>(withNewline.size()), &written, nullptr);
        }

        int WaitForAny(const std::vector<std::string>& markers, std::chrono::milliseconds timeout) const
        {
            const auto deadline = std::chrono::steady_clock::now() + timeout;
            while (true)
            {
                {
                    std::lock_guard lock(state_->bufferMutex);
                    size_t          bestPosition = std::string::npos;
                    int             bestIndex    = -1;
                    for (size_t i = 0; i < markers.size(); ++i)
                    {
                        const auto position = state_->buffer.find(markers[i], state_->searchCursor);
                        if (position != std::string::npos && position < bestPosition)
                        {
                            bestPosition = position;
                            bestIndex    = static_cast<int>(i);
                        }
                    }
                    if (bestIndex != -1)
                    {
                        state_->searchCursor = bestPosition + markers[bestIndex].size();
                        return bestIndex;
                    }
                }

                if (std::chrono::steady_clock::now() >= deadline)
                {
                    return -1;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
            }
        }

        bool WaitFor(const std::string& marker, std::chrono::milliseconds timeout) const
        {
            return WaitForAny({ marker }, timeout) == 0;
        }

        void Kill() const
        {
            TerminateProcess(state_->processInfo.hProcess, 1);
        }

        bool IsRunning() const
        {
            DWORD exitCode = STILL_ACTIVE;
            return GetExitCodeProcess(state_->processInfo.hProcess, &exitCode) && exitCode == STILL_ACTIVE;
        }

        std::optional<unsigned long> WaitForExit(std::chrono::milliseconds timeout) const
        {
            if (WaitForSingleObject(state_->processInfo.hProcess, static_cast<DWORD>(timeout.count())) != WAIT_OBJECT_0)
            {
                return std::nullopt;
            }
            DWORD exitCode = 0;
            if (!GetExitCodeProcess(state_->processInfo.hProcess, &exitCode))
            {
                return std::nullopt;
            }
            return exitCode;
        }

        std::string GetAccumulatedOutput() const
        {
            std::lock_guard lock(state_->bufferMutex);
            return state_->buffer;
        }

    private:
        [[noreturn]] static void ThrowLastError(const std::string& what)
        {
            throw std::runtime_error(what + " (GetLastError=" + std::to_string(GetLastError()) + ")");
        }

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

        std::unique_ptr<ConsoleProcessState> state_;

        explicit ConsoleProcess(std::unique_ptr<ConsoleProcessState> state)
            : state_(std::move(state))
        {
        }
    };
} // namespace Chess::E2E
