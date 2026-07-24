#include <gtest/gtest.h>
#include <optional>
import Chess.Core.ePieceType;
import Chess.Core.PieceTypeConverter;

namespace ServerTests
{
    TEST(PieceTypeConverterTests, ConvertsKnownPieceTypesToDisplayValues)
    {
        EXPECT_EQ(Chess::Core::PieceTypeConverter::TryConvertToChar(Chess::Core::ePieceType::BISHOP), std::optional<char>('B'));
        EXPECT_EQ(Chess::Core::PieceTypeConverter::ConvertToString(Chess::Core::ePieceType::KNIGHT), "Knight");
        EXPECT_EQ(Chess::Core::PieceTypeConverter::ConvertToConfigString(Chess::Core::ePieceType::QUEEN), "queens");
    }

    TEST(PieceTypeConverterTests, HandlesUnknownEnumValues)
    {
        EXPECT_EQ(Chess::Core::PieceTypeConverter::TryConvertToChar(static_cast<Chess::Core::ePieceType>(99)), std::nullopt);
        EXPECT_EQ(Chess::Core::PieceTypeConverter::ConvertToString(static_cast<Chess::Core::ePieceType>(99)), "Unknown");
        EXPECT_EQ(Chess::Core::PieceTypeConverter::ConvertToConfigString(static_cast<Chess::Core::ePieceType>(99)), "unknown");
    }
} // namespace ServerTests
