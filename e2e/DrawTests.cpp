#include <chrono>
#include <gtest/gtest.h>

import Chess.E2E.ChessMoveTyper;
import Chess.E2E.GameHarness;
import Chess.E2E.MoveOutcome;
import Chess.E2E.Square;

namespace E2ETests
{
    TEST(DrawTests, NoProgressRuleEndsTheGameAsADraw)
    {
        auto        harness = Chess::E2E::GameHarness::Start();
        const auto& white   = harness.White();
        const auto& black   = harness.Black();

        constexpr int PLY_COUNT = 150;

        bool whiteKnightOnF3 = false;
        bool blackKnightOnF6 = false;

        for (int ply = 0; ply < PLY_COUNT; ++ply)
        {
            const bool              isLastPly = (ply == PLY_COUNT - 1);
            Chess::E2E::MoveOutcome outcome;

            if (ply % 2 == 0)
            {
                const auto from = whiteKnightOnF3 ? Chess::E2E::Square{ .file = 'F', .rank = 3 } : Chess::E2E::Square{ .file = 'G', .rank = 1 };
                const auto to   = whiteKnightOnF3 ? Chess::E2E::Square{ .file = 'G', .rank = 1 } : Chess::E2E::Square{ .file = 'F', .rank = 3 };
                outcome         = isLastPly ? Chess::E2E::ChessMoveTyper::PlayMove(white, from, to, std::nullopt, std::chrono::seconds(10))
                                            : Chess::E2E::ChessMoveTyper::TypeMove(white, from, to);
                whiteKnightOnF3 = !whiteKnightOnF3;
            }
            else
            {
                const auto from = blackKnightOnF6 ? Chess::E2E::Square{ .file = 'F', .rank = 6 } : Chess::E2E::Square{ .file = 'G', .rank = 8 };
                const auto to   = blackKnightOnF6 ? Chess::E2E::Square{ .file = 'G', .rank = 8 } : Chess::E2E::Square{ .file = 'F', .rank = 6 };
                outcome         = isLastPly ? Chess::E2E::ChessMoveTyper::PlayMove(black, from, to, std::nullopt, std::chrono::seconds(10))
                                            : Chess::E2E::ChessMoveTyper::TypeMove(black, from, to);
                blackKnightOnF6 = !blackKnightOnF6;
            }

            if (isLastPly)
            {
                ASSERT_EQ(outcome, Chess::E2E::MoveOutcome::Draw);
            }
            else
            {
                ASSERT_EQ(outcome, Chess::E2E::MoveOutcome::Ongoing) << "ply " << ply << " was rejected as soon as it was typed";
            }
        }

        const auto whiteWakeUpOutcome = Chess::E2E::ChessMoveTyper::PlayMove(
            white,
            Chess::E2E::Square{ .file = 'F', .rank = 3 },
            Chess::E2E::Square{ .file = 'G', .rank = 1 },
            std::nullopt,
            std::chrono::seconds(10));
        ASSERT_EQ(whiteWakeUpOutcome, Chess::E2E::MoveOutcome::Draw);

        const auto whiteExit = white.WaitForExit(std::chrono::seconds(5));
        const auto blackExit = black.WaitForExit(std::chrono::seconds(5));
        ASSERT_TRUE(whiteExit.has_value());
        ASSERT_TRUE(blackExit.has_value());
        EXPECT_EQ(whiteExit.value(), 0u);
        EXPECT_EQ(blackExit.value(), 0u);
    }
} // namespace E2ETests
