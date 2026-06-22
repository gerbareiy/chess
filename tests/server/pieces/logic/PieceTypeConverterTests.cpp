#include <gtest/gtest.h>

#include <optional>

import Chess.ePieceType;
import Chess.PieceTypeConverter;

namespace ServerTests
{
    TEST(PieceTypeConverterTests, ConvertsKnownPieceTypesToDisplayValues)
    {
        EXPECT_EQ(Chess::PieceTypeConverter::TryConvertToChar(Chess::ePieceType::BISHOP), std::optional<char>('B'));
        EXPECT_EQ(Chess::PieceTypeConverter::ConvertToString(Chess::ePieceType::KNIGHT), "Knight");
        EXPECT_EQ(Chess::PieceTypeConverter::ConvertToConfigString(Chess::ePieceType::QUEEN), "queens");
    }

    TEST(PieceTypeConverterTests, HandlesUnknownEnumValues)
    {
        EXPECT_EQ(Chess::PieceTypeConverter::TryConvertToChar(static_cast<Chess::ePieceType>(99)), std::nullopt);
        EXPECT_EQ(Chess::PieceTypeConverter::ConvertToString(static_cast<Chess::ePieceType>(99)), "Unknown");
        EXPECT_EQ(Chess::PieceTypeConverter::ConvertToConfigString(static_cast<Chess::ePieceType>(99)), "unknown");
    }
} // namespace ServerTests
