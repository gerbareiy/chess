#include <gtest/gtest.h>
import Chess.ePieceColor;
import Chess.PieceColorConverter;

namespace ServerTests
{
    TEST(PieceColorConverterTests, ConvertsKnownPieceColorsToDisplayValues)
    {
        EXPECT_EQ(Chess::PieceColorConverter::ConvertToString(Chess::ePieceColor::WHITE), "w");
        EXPECT_EQ(Chess::PieceColorConverter::ConvertToString(Chess::ePieceColor::BLACK), "b");
    }

    TEST(PieceColorConverterTests, HandlesUnknownEnumValues)
    {
        EXPECT_EQ(Chess::PieceColorConverter::ConvertToString(static_cast<Chess::ePieceColor>(99)), "Unknown color");
    }
} // namespace ServerTests
