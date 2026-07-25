#include <chrono>
#include <gtest/gtest.h>

import Chess.E2E.ChessMoveTyper;
import Chess.E2E.GameHarness;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace E2ETests
{
    TEST(FoolsMateTests, FastestCheckmateEndsGameForBothPlayers)
    {
        auto harness = Chess::E2E::GameHarness::Start();

        EXPECT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'F', .rank = 2 }, Chess::E2E::Square{ .file = 'F', .rank = 3 }),
            Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'E', .rank = 7 }, Chess::E2E::Square{ .file = 'E', .rank = 5 }),
            Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'G', .rank = 2 }, Chess::E2E::Square{ .file = 'G', .rank = 4 }),
            Chess::E2E::MoveOutcome::Ongoing);

        const auto matingMoveOutcome = Chess::E2E::ChessMoveTyper::PlayMove(
            harness.Black(), Chess::E2E::Square{ .file = 'D', .rank = 8 }, Chess::E2E::Square{ .file = 'H', .rank = 4 });
        EXPECT_EQ(matingMoveOutcome, Chess::E2E::MoveOutcome::Checkmate);

        EXPECT_TRUE(harness.White().WaitFor("Checkmate!", std::chrono::seconds(10)));

        const auto whiteExit = harness.White().WaitForExit(std::chrono::seconds(5));
        const auto blackExit = harness.Black().WaitForExit(std::chrono::seconds(5));
        ASSERT_TRUE(whiteExit.has_value());
        ASSERT_TRUE(blackExit.has_value());
        EXPECT_EQ(whiteExit.value(), 0u);
        EXPECT_EQ(blackExit.value(), 0u);
    }
} // namespace E2ETests
