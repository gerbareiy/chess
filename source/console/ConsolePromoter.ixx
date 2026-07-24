module;
#include <boost/signals2.hpp>
#include <cctype>
#include <iostream>
#include <string>
export module Chess.Console.ConsolePromoter;
import Chess.Core.eInputType;
import Chess.Core.ePieceType;
import Chess.Core.PieceTypeConverter;
import Chess.Core.Promoter;
import Chess.Utils.ConsoleReader;

namespace Chess::Console
{
    export class ConsolePromoter final : public ::Chess::Core::Promoter
    {
    public:
        virtual ::Chess::Core::ePieceType GetPromoteType() const override
        {
            while (true)
            {
                GetSignalOnEnter()(::Chess::Core::eInputType::PROMOTION);

                const auto input      = EnterPromotionType();
                const auto normalized = TryNormalizePromotionChoice(input);
                if (normalized == ::Chess::Core::PieceTypeConverter::TryConvertToChar(::Chess::Core::ePieceType::BISHOP))
                {
                    return ::Chess::Core::ePieceType::BISHOP;
                }
                if (normalized == ::Chess::Core::PieceTypeConverter::TryConvertToChar(::Chess::Core::ePieceType::KNIGHT))
                {
                    return ::Chess::Core::ePieceType::KNIGHT;
                }
                if (normalized == ::Chess::Core::PieceTypeConverter::TryConvertToChar(::Chess::Core::ePieceType::QUEEN))
                {
                    return ::Chess::Core::ePieceType::QUEEN;
                }
                if (normalized == ::Chess::Core::PieceTypeConverter::TryConvertToChar(::Chess::Core::ePieceType::ROOK))
                {
                    return ::Chess::Core::ePieceType::ROOK;
                }
            }
        }

    private:
        static char GuessPromotionSymbol(char symbol)
        {
            const char upped = static_cast<char>(std::toupper(symbol));
            if (upped == ::Chess::Core::PieceTypeConverter::TryConvertToChar(::Chess::Core::ePieceType::KING))
            {
                return ::Chess::Core::PieceTypeConverter::TryConvertToChar(::Chess::Core::ePieceType::KNIGHT).value();
            }
            return upped;
        }

        static std::optional<char> TryNormalizePromotionChoice(const std::string& input)
        {
            for (const unsigned char symbol : input)
            {
                if (!std::isspace(symbol))
                {
                    return GuessPromotionSymbol(symbol);
                }
            }
            return std::nullopt;
        }

        static std::string EnterPromotionType()
        {
            auto result = ::Chess::Utils::ConsoleReader::ReadLine();
            std::getline(std::cin, result);
            return result;
        }
    };
} // namespace Chess::Console
