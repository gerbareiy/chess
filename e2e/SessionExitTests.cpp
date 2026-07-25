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
    TEST(SessionExitTests, SurvivorIsToldWhenTheOpponentLeavesMidGame)
    {
        auto        harness = Chess::E2E::GameHarness::Start();
        const auto& white   = harness.White();
        const auto& black   = harness.Black();

        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(white, Chess::E2E::Square{ .file = 'E', .rank = 2 }, Chess::E2E::Square{ .file = 'E', .rank = 4 }),
            Chess::E2E::MoveOutcome::Ongoing);
        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(black, Chess::E2E::Square{ .file = 'E', .rank = 7 }, Chess::E2E::Square{ .file = 'E', .rank = 5 }),
            Chess::E2E::MoveOutcome::Ongoing);
        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(white, Chess::E2E::Square{ .file = 'G', .rank = 1 }, Chess::E2E::Square{ .file = 'F', .rank = 3 }),
            Chess::E2E::MoveOutcome::Ongoing);

        // White has just moved, so it is blocked on the network waiting for Black's reply.
        black.Kill();

        EXPECT_TRUE(white.WaitFor("Opponent left the game.", std::chrono::seconds(15)));

        const auto whiteExit = white.WaitForExit(std::chrono::seconds(10));
        ASSERT_TRUE(whiteExit.has_value());
        EXPECT_EQ(whiteExit.value(), 0u);
    }

    // A player whose own turn it is sits in a blocking stdin read, so it is not listening to
    // the server at that moment: it learns the opponent left as soon as it submits its move,
    // not while it is still typing it.
    TEST(SessionExitTests, PlayerOnMoveIsToldOnceItSubmitsTheMove)
    {
        auto        harness = Chess::E2E::GameHarness::Start();
        const auto& white   = harness.White();

        harness.Black().Kill();

        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::TypeMove(white, Chess::E2E::Square{ .file = 'E', .rank = 2 }, Chess::E2E::Square{ .file = 'E', .rank = 4 }),
            Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_TRUE(white.WaitFor("Opponent left the game.", std::chrono::seconds(15)));

        const auto whiteExit = white.WaitForExit(std::chrono::seconds(10));
        ASSERT_TRUE(whiteExit.has_value());
        EXPECT_EQ(whiteExit.value(), 0u);
    }

    TEST(SessionExitTests, ServerKeepsServingAfterAPlayerLeaves)
    {
        auto harness = Chess::E2E::GameHarness::Start();

        harness.Black().Kill();

        const std::vector<std::string> clientArguments = { "127.0.0.1", std::to_string(harness.GetPort()) };

        const auto nextWhite = Chess::E2E::ConsoleProcess::Start(
            Chess::E2E::ChessExecutables::GetConsolePath(), Chess::E2E::ChessExecutables::GetOwnDirectory(), clientArguments);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        const auto nextBlack = Chess::E2E::ConsoleProcess::Start(
            Chess::E2E::ChessExecutables::GetConsolePath(), Chess::E2E::ChessExecutables::GetOwnDirectory(), clientArguments);

        ASSERT_TRUE(nextWhite.WaitFor("You play White.", std::chrono::seconds(10)));
        ASSERT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                nextWhite, Chess::E2E::Square{ .file = 'E', .rank = 2 }, Chess::E2E::Square{ .file = 'E', .rank = 4 }),
            Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_TRUE(nextBlack.WaitFor("File: ", std::chrono::seconds(10)));
    }
} // namespace E2ETests
