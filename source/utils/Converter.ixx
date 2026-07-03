module;
#include <charconv>
#include <optional>
#include <string>
export module Chess.Utils.Converter;

namespace Chess::Utils
{
    export class Converter
    {
    public:
        static std::optional<uint32_t> ToUint32(const std::string& toConvert)
        {
            uint32_t result                = 0;
            auto [pointer, errorCondition] = std::from_chars(toConvert.data(), toConvert.data() + toConvert.size(), result);
            if (errorCondition != std::errc() || pointer != toConvert.data() + toConvert.size())
            {
                return std::nullopt;
            }
            return result;
        }

        static std::optional<int32_t> ToInt32(const std::string& toConvert)
        {
            int32_t result                 = 0;
            auto [pointer, errorCondition] = std::from_chars(toConvert.data(), toConvert.data() + toConvert.size(), result);
            if (errorCondition != std::errc() || pointer != toConvert.data() + toConvert.size())
            {
                return std::nullopt;
            }
            return result;
        }
    };
} // namespace Chess::Utils
