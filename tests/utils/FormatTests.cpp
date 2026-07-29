#include <format>
#include <gtest/gtest.h>
#include <string>
#include <string_view>
import Chess.Utils.Exceptions;
import Chess.Utils.Format;

namespace ServerTests
{
    using Chess::Utils::Format;
    using Chess::Utils::FormatException;

    TEST(FormatTests, CompileTimeCheckedFormatsArguments)
    {
        EXPECT_EQ(Format::Text("{} to {}", "E2", "E4"), "E2 to E4");
    }

    TEST(FormatTests, CompileTimeCheckedHonoursFormatSpecifiers)
    {
        EXPECT_EQ(Format::Text("{:03}", 7), "007");
    }

    TEST(FormatTests, CompileTimeCheckedFormatsWithoutArguments)
    {
        EXPECT_EQ(Format::Text("no placeholders"), "no placeholders");
    }

    TEST(FormatTests, ConstexprCharArrayStaysCompileTimeChecked)
    {
        static constexpr char formatString[] = "{} to {}";
        EXPECT_EQ(Format::Text(formatString, "E2", "E4"), "E2 to E4");
    }

    TEST(FormatTests, RuntimeFormatsArguments)
    {
        const std::string formatString = "{} to {}";
        EXPECT_EQ(Format::Text(formatString, "E2", "E4"), "E2 to E4");
    }

    TEST(FormatTests, RuntimeAcceptsExplicitArgumentIndices)
    {
        const std::string formatString = "{1} then {0}";
        EXPECT_EQ(Format::Text(formatString, 1, 2), "2 then 1");
    }

    TEST(FormatTests, StringViewTakesTheRuntimePath)
    {
        constexpr std::string_view formatString = "{} and {}";
        EXPECT_THROW(Format::Text(formatString, 1), FormatException);
    }

    TEST(FormatTests, CharPointerTakesTheRuntimePath)
    {
        const auto formatString = "{} and {}";
        EXPECT_THROW(Format::Text(formatString, 1), FormatException);
    }

    TEST(FormatTests, MutableCharPointerTakesTheRuntimePath)
    {
        char  storage[]    = "{} and {}";
        char* formatString = storage;
        EXPECT_THROW(Format::Text(formatString, 1), FormatException);
    }

    TEST(FormatTests, RuntimeThrowsFormatExceptionOnMissingArgument)
    {
        const std::string formatString = "{} and {}";
        EXPECT_THROW(Format::Text(formatString, 1), FormatException);
    }

    TEST(FormatTests, RuntimeThrowsFormatExceptionOnArgumentIndexPastTheEnd)
    {
        const std::string formatString = "{7}";
        EXPECT_THROW(Format::Text(formatString, 1), FormatException);
    }

    TEST(FormatTests, RuntimeThrowsFormatExceptionOnUnclosedPlaceholder)
    {
        const std::string formatString = "{";
        EXPECT_THROW(Format::Text(formatString), FormatException);
    }

    TEST(FormatTests, RuntimeThrowsFormatExceptionOnSpecifierTheArgumentDoesNotSupport)
    {
        const std::string formatString = "{:d}";
        EXPECT_THROW(Format::Text(formatString, "not a number"), FormatException);
    }

    TEST(FormatTests, RuntimeFormatExceptionIsCatchableAsStdRuntimeError)
    {
        const std::string formatString = "{} and {}";
        EXPECT_THROW(Format::Text(formatString, 1), std::runtime_error);
    }

    TEST(FormatTests, RuntimeFormatExceptionReportsFormatStringAndReason)
    {
        const std::string formatString = "{} and {}";

        try
        {
            Format::Text(formatString, 1);
            FAIL() << "expected a FormatException";
        }
        catch (const FormatException& exception)
        {
            const std::string_view message = exception.what();
            EXPECT_NE(message.find(formatString), std::string_view::npos);
            EXPECT_NE(message.find("Format error"), std::string_view::npos);
        }
    }

    TEST(FormatTests, RuntimeSucceedsOnAValidStringWithoutThrowing)
    {
        const std::string formatString = "{:>5}";
        EXPECT_NO_THROW(EXPECT_EQ(Format::Text(formatString, "ab"), "   ab"));
    }
} // namespace ServerTests
