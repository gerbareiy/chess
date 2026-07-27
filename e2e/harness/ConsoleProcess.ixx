module;
#include <chrono>
#include <filesystem>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <utility>
#include <vector>
export module Chess.E2E.ConsoleProcess;
import Chess.E2E.ChildProcess;

namespace Chess::E2E
{
    struct ConsoleProcessState
    {
        ChildProcess process;
        std::thread  readerThread;
        std::mutex   bufferMutex;
        std::string  buffer;
        size_t       searchCursor = 0;

        explicit ConsoleProcessState(ChildProcess started)
            : process(std::move(started))
        {
        }

        ~ConsoleProcessState()
        {
            if (process.IsRunning())
            {
                process.Kill();
            }

            // Closing the input and killing the child both end its output, which is what lets the reader finish.
            process.CloseInput();
            if (readerThread.joinable())
            {
                readerThread.join();
            }
        }
    };

    export class ConsoleProcess
    {
    public:
        static ConsoleProcess Start(
            const std::filesystem::path& exePath, const std::filesystem::path& workingDirectory, const std::vector<std::string>& arguments = {})
        {
            auto state = std::make_unique<ConsoleProcessState>(ChildProcess::Start(exePath, workingDirectory, arguments));

            ConsoleProcessState* rawState = state.get();
            state->readerThread           = std::thread(
                [rawState]
                {
                    char buffer[READ_BUFFER_SIZE];
                    while (true)
                    {
                        const auto bytesRead = rawState->process.Read(buffer, sizeof(buffer));
                        if (bytesRead == 0)
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
            state_->process.Write(line + "\n");
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
                std::this_thread::sleep_for(POLL_INTERVAL);
            }
        }

        bool WaitFor(const std::string& marker, std::chrono::milliseconds timeout) const
        {
            return WaitForAny({ marker }, timeout) == 0;
        }

        void Kill() const
        {
            state_->process.Kill();
        }

        bool IsRunning() const
        {
            return state_->process.IsRunning();
        }

        std::optional<unsigned long> WaitForExit(std::chrono::milliseconds timeout) const
        {
            return state_->process.TryWaitForExit(timeout);
        }

        std::string GetAccumulatedOutput() const
        {
            std::lock_guard lock(state_->bufferMutex);
            return state_->buffer;
        }

    private:
        static constexpr size_t                    READ_BUFFER_SIZE = 4096;
        static constexpr std::chrono::milliseconds POLL_INTERVAL    = std::chrono::milliseconds(15);

        std::unique_ptr<ConsoleProcessState> state_;

        explicit ConsoleProcess(std::unique_ptr<ConsoleProcessState> state)
            : state_(std::move(state))
        {
        }
    };
} // namespace Chess::E2E
