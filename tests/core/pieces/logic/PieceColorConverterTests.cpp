#include <gtest/gtest.h>
import Chess.Core.ePieceColor;
import Chess.Core.PieceColorConverter;

namespace ServerTests
{
    TEST(PieceColorConverterTests, ConvertsKnownPieceColorsToDisplayValues)
    {
        EXPECT_EQ(Chess::Core::PieceColorConverter::ConvertToString(Chess::Core::ePieceColor::WHITE), "w");
        EXPECT_EQ(Chess::Core::PieceColorConverter::ConvertToString(Chess::Core::ePieceColor::BLACK), "b");
    }

    TEST(PieceColorConverterTests, HandlesUnknownEnumValues)
    {
        EXPECT_EQ(Chess::Core::PieceColorConverter::ConvertToString(static_cast<Chess::Core::ePieceColor>(99)), "Unknown color");
    }
} // namespace ServerTests
