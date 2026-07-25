module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <atomic>
#include <chrono>
#include <ranges>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
export module Chess.E2E.GameHarness;
import Chess.E2E.ChessExecutables;
import Chess.E2E.ConsoleProcess;

namespace Chess::E2E
{
    export class GameHarness
    {
    public:
        static GameHarness Start()
        {
            ChessExecutables::RequireLaunchableBinaries();

            auto [server, port] = StartServerOnFreePort();

            const auto  portArgument = std::to_string(port);
            const auto& consolePath  = ChessExecutables::GetConsolePath();
            const auto& directory    = ChessExecutables::GetOwnDirectory();

            auto white = ConsoleProcess::Start(consolePath, directory, { LOOPBACK_HOST, portArgument });
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            auto black = ConsoleProcess::Start(consolePath, directory, { LOOPBACK_HOST, portArgument });

            if (!white.WaitFor("You play White.", std::chrono::seconds(10)))
            {
                throw std::runtime_error("first console client did not become White on port " + portArgument);
            }

            return GameHarness(std::move(server), std::move(white), std::move(black), port);
        }

        GameHarness(GameHarness&&) noexcept            = default;
        GameHarness& operator=(GameHarness&&) noexcept = default;
        GameHarness(const GameHarness&)                = delete;
        GameHarness& operator=(const GameHarness&)     = delete;

        ConsoleProcess& Server()
        {
            return server_;
        }

        ConsoleProcess& White()
        {
            return white_;
        }

        ConsoleProcess& Black()
        {
            return black_;
        }

        unsigned short GetPort() const
        {
            return port_;
        }

    private:
        static constexpr int            CANDIDATE_PORTS_COUNT = 50;
        static constexpr unsigned short LOWEST_CANDIDATE_PORT = 20000;
        static constexpr unsigned short CANDIDATE_PORTS_RANGE = 20000;
        static inline const std::string LOOPBACK_HOST         = "127.0.0.1";

        ConsoleProcess server_;
        ConsoleProcess white_;
        ConsoleProcess black_;
        unsigned short port_ = 0;

        GameHarness(ConsoleProcess server, ConsoleProcess white, ConsoleProcess black, unsigned short port)
            : server_(std::move(server))
            , white_(std::move(white))
            , black_(std::move(black))
            , port_(port)
        {
        }

        static unsigned short NextCandidatePort()
        {
            static std::atomic<unsigned short> offset = static_cast<unsigned short>(GetCurrentProcessId() % CANDIDATE_PORTS_RANGE);
            return static_cast<unsigned short>(LOWEST_CANDIDATE_PORT + offset++ % CANDIDATE_PORTS_RANGE);
        }

        static std::pair<ConsoleProcess, unsigned short> StartServerOnFreePort()
        {
            std::string    lastOutput;
            unsigned short lastPort = 0;
            for (int candidate : std::views::iota(0, CANDIDATE_PORTS_COUNT))
            {
                lastPort = NextCandidatePort();
                auto server =
                    ConsoleProcess::Start(ChessExecutables::GetServerPath(), ChessExecutables::GetOwnDirectory(), { std::to_string(lastPort) });

                if (!server.WaitForExit(std::chrono::milliseconds(750)).has_value())
                {
                    return { std::move(server), lastPort };
                }

                lastOutput = server.GetAccumulatedOutput();
            }
            throw std::runtime_error(
                "server.exe exited immediately on all " + std::to_string(CANDIDATE_PORTS_COUNT)
                + " candidate ports (last tried: " + std::to_string(lastPort) + "). Path: " + ChessExecutables::GetServerPath().string()
                + ", working directory: " + ChessExecutables::GetOwnDirectory().string() + ", its output: [" + lastOutput + "]");
        }
    };
} // namespace Chess::E2E
