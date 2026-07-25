#include <gtest/gtest.h>
#include <string>

import Chess.E2E.ChessMoveTyper;
import Chess.E2E.GameHarness;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace E2ETests
{
    TEST(LongGameTests, SustainsAtLeastOneHundredPlies)
    {
        auto        harness = Chess::E2E::GameHarness::Start();
        const auto& white   = harness.White();
        const auto& black   = harness.Black();

        constexpr int PLY_COUNT = 110;

        bool whiteKnightOnF3 = false;
        bool blackKnightOnF6 = false;

        for (int ply = 0; ply < PLY_COUNT; ++ply)
        {
            Chess::E2E::MoveOutcome outcome;
            if (ply % 2 == 0)
            {
                const auto from = whiteKnightOnF3 ? Chess::E2E::Square{ .file = 'F', .rank = 3 } : Chess::E2E::Square{ .file = 'G', .rank = 1 };
                const auto to   = whiteKnightOnF3 ? Chess::E2E::Square{ .file = 'G', .rank = 1 } : Chess::E2E::Square{ .file = 'F', .rank = 3 };
                outcome         = Chess::E2E::ChessMoveTyper::TypeMove(white, from, to);
                whiteKnightOnF3 = !whiteKnightOnF3;
            }
            else
            {
                const auto from = blackKnightOnF6 ? Chess::E2E::Square{ .file = 'F', .rank = 6 } : Chess::E2E::Square{ .file = 'G', .rank = 8 };
                const auto to   = blackKnightOnF6 ? Chess::E2E::Square{ .file = 'G', .rank = 8 } : Chess::E2E::Square{ .file = 'F', .rank = 6 };
                outcome         = Chess::E2E::ChessMoveTyper::TypeMove(black, from, to);
                blackKnightOnF6 = !blackKnightOnF6;
            }

            ASSERT_EQ(outcome, Chess::E2E::MoveOutcome::Ongoing) << "ply " << ply << " was rejected as soon as it was typed";
        }

        EXPECT_TRUE(white.IsRunning());
        EXPECT_TRUE(black.IsRunning());

        const auto whiteOutput = white.GetAccumulatedOutput();
        EXPECT_EQ(whiteOutput.find("Checkmate!"), std::string::npos);
        EXPECT_EQ(whiteOutput.find("Draw!"), std::string::npos);
        EXPECT_EQ(whiteOutput.find("Illegal move."), std::string::npos);

        const auto blackOutput = black.GetAccumulatedOutput();
        EXPECT_EQ(blackOutput.find("Illegal move."), std::string::npos);
    }
} // namespace E2ETests
