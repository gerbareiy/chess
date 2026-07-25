#include <gtest/gtest.h>

import Chess.E2E.ChessMoveTyper;
import Chess.E2E.GameHarness;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace E2ETests
{
    namespace
    {
        void ExpectOngoing(Chess::E2E::MoveOutcome outcome)
        {
            EXPECT_EQ(outcome, Chess::E2E::MoveOutcome::Ongoing);
        }
    } // namespace

    TEST(CastlingTests, BothSidesCastleKingside)
    {
        auto harness = Chess::E2E::GameHarness::Start();

        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'E', .rank = 2 }, Chess::E2E::Square{ .file = 'E', .rank = 4 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'E', .rank = 7 }, Chess::E2E::Square{ .file = 'E', .rank = 5 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'G', .rank = 1 }, Chess::E2E::Square{ .file = 'F', .rank = 3 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'B', .rank = 8 }, Chess::E2E::Square{ .file = 'C', .rank = 6 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'F', .rank = 1 }, Chess::E2E::Square{ .file = 'C', .rank = 4 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'F', .rank = 8 }, Chess::E2E::Square{ .file = 'C', .rank = 5 }));

        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'E', .rank = 1 }, Chess::E2E::Square{ .file = 'G', .rank = 1 }));

        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'G', .rank = 8 }, Chess::E2E::Square{ .file = 'F', .rank = 6 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'D', .rank = 2 }, Chess::E2E::Square{ .file = 'D', .rank = 3 }));

        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'E', .rank = 8 }, Chess::E2E::Square{ .file = 'G', .rank = 8 }));
    }

    TEST(CastlingTests, WhiteCastlesQueenside)
    {
        auto harness = Chess::E2E::GameHarness::Start();

        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'D', .rank = 2 }, Chess::E2E::Square{ .file = 'D', .rank = 4 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'D', .rank = 7 }, Chess::E2E::Square{ .file = 'D', .rank = 5 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'B', .rank = 1 }, Chess::E2E::Square{ .file = 'C', .rank = 3 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'B', .rank = 8 }, Chess::E2E::Square{ .file = 'C', .rank = 6 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'C', .rank = 1 }, Chess::E2E::Square{ .file = 'F', .rank = 4 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'C', .rank = 8 }, Chess::E2E::Square{ .file = 'F', .rank = 5 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'D', .rank = 1 }, Chess::E2E::Square{ .file = 'D', .rank = 2 }));
        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.Black(), Chess::E2E::Square{ .file = 'D', .rank = 8 }, Chess::E2E::Square{ .file = 'D', .rank = 7 }));

        ExpectOngoing(
            Chess::E2E::ChessMoveTyper::PlayMove(
                harness.White(), Chess::E2E::Square{ .file = 'E', .rank = 1 }, Chess::E2E::Square{ .file = 'C', .rank = 1 }));
    }
} // namespace E2ETests
