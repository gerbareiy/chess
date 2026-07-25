#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <thread>
#include <vector>

import Chess.E2E.ChessExecutables;
import Chess.E2E.ChessMoveTyper;
import Chess.E2E.ConsoleProcess;
import Chess.E2E.GameHarness;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace E2ETests
{
    TEST(ConcurrentSessionsTests, OneServerPlaysTwoGamesAtTheSameTime)
    {
        auto        harness    = Chess::E2E::GameHarness::Start();
        const auto& firstWhite = harness.White();
        const auto& firstBlack = harness.Black();

        const std::vector<std::string> clientArguments = { "127.0.0.1", std::to_string(harness.GetPort()) };
        const auto&                    consolePath     = Chess::E2E::ChessExecutables::GetConsolePath();
        const auto&                    directory       = Chess::E2E::ChessExecutables::GetOwnDirectory();

        const auto secondWhite = Chess::E2E::ConsoleProcess::Start(consolePath, directory, clientArguments);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        const auto secondBlack = Chess::E2E::ConsoleProcess::Start(consolePath, directory, clientArguments);

        // The second pair is given a game while the first match is still live - the whole
        // point of the change, and impossible before it.
        ASSERT_TRUE(secondWhite.WaitFor("You play White.", std::chrono::seconds(10)));

        // Play the second game all the way to checkmate without touching the first.
        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                secondWhite, Chess::E2E::Square{ .file = 'F', .rank = 2 }, Chess::E2E::Square{ .file = 'F', .rank = 3 }),
            Chess::E2E::MoveOutcome::Ongoing);
        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                secondBlack, Chess::E2E::Square{ .file = 'E', .rank = 7 }, Chess::E2E::Square{ .file = 'E', .rank = 5 }),
            Chess::E2E::MoveOutcome::Ongoing);
        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                secondWhite, Chess::E2E::Square{ .file = 'G', .rank = 2 }, Chess::E2E::Square{ .file = 'G', .rank = 4 }),
            Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                secondBlack, Chess::E2E::Square{ .file = 'D', .rank = 8 }, Chess::E2E::Square{ .file = 'H', .rank = 4 }),
            Chess::E2E::MoveOutcome::Checkmate);
        EXPECT_TRUE(secondWhite.WaitFor("Checkmate!", std::chrono::seconds(10)));

        // The first match was never disturbed by the second one starting, finishing, or its
        // players disconnecting, and is still playable.
        EXPECT_TRUE(firstWhite.IsRunning());
        EXPECT_TRUE(firstBlack.IsRunning());
        EXPECT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                firstWhite, Chess::E2E::Square{ .file = 'E', .rank = 2 }, Chess::E2E::Square{ .file = 'E', .rank = 4 }),
            Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                firstBlack, Chess::E2E::Square{ .file = 'E', .rank = 7 }, Chess::E2E::Square{ .file = 'E', .rank = 5 }),
            Chess::E2E::MoveOutcome::Ongoing);
    }
} // namespace E2ETests
