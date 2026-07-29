module;
#include <concepts>
#include <format>
#include <string>
#include <string_view>
#include <utility>
export module Chess.Utils.Format;
import Chess.Utils.Exceptions;

namespace Chess::Utils
{
    template <typename T>
    concept Formattable = std::convertible_to<T, std::string_view>;

    export class Format
    {
    public:
        template <typename... Args> static std::string Text(std::format_string<Args...> toFormat, Args&&... args)
        {
            return std::format(toFormat, std::forward<Args>(args)...);
        }

        template <Formattable FormatString, typename... Args> static std::string Text(const FormatString& toFormat, Args&&... args)
        {
            try
            {
                return std::vformat(std::string_view(toFormat), std::make_format_args(args...));
            }
            catch (const std::format_error& error)
            {
                throw FormatException(std::string(std::string_view(toFormat)), error.what());
            }
        }
    };
} // namespace Chess::Utils
