#include <gtest/gtest.h>

import Chess.E2E.ChessMoveTyper;
import Chess.E2E.ConsoleProcess;
import Chess.E2E.GameHarness;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace E2ETests
{
    namespace
    {
        Chess::E2E::MoveOutcome Move(Chess::E2E::ConsoleProcess& process, char fromFile, int fromRank, char toFile, int toRank)
        {
            return Chess::E2E::ChessMoveTyper::PlayMove(
                process, Chess::E2E::Square{ .file = fromFile, .rank = fromRank }, Chess::E2E::Square{ .file = toFile, .rank = toRank });
        }
    } // namespace

    TEST(EnPassantTests, CapturesEnPassantSeveralTimesInARow)
    {
        auto  harness = Chess::E2E::GameHarness::Start();
        auto& white   = harness.White();
        auto& black   = harness.Black();

        EXPECT_EQ(Move(white, 'E', 2, 'E', 4), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'A', 7, 'A', 6), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'E', 4, 'E', 5), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'D', 7, 'D', 5), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'E', 5, 'D', 6), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'G', 8, 'F', 6), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'G', 1, 'F', 3), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'C', 7, 'C', 5), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'B', 1, 'C', 3), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'C', 5, 'C', 4), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'B', 2, 'B', 4), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'C', 4, 'B', 3), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'H', 2, 'H', 4), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'E', 7, 'E', 6), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'H', 4, 'H', 5), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'G', 7, 'G', 5), Chess::E2E::MoveOutcome::Ongoing);

        EXPECT_EQ(Move(white, 'H', 5, 'G', 6), Chess::E2E::MoveOutcome::Ongoing);
        EXPECT_EQ(Move(black, 'F', 8, 'E', 7), Chess::E2E::MoveOutcome::Ongoing);
    }
} // namespace E2ETests
