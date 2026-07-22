module;
#include <boost/signals2.hpp>
#include <cctype>
#include <iostream>
#include <string>
export module Console.Chess.ConsolePromoter;
import Chess.eInputType;
import Chess.ePieceType;
import Chess.Promoter;
import Chess.PieceTypeConverter;
import Chess.Utils.ConsoleReader;

namespace Console::Chess
{
    export class ConsolePromoter final : public ::Chess::Promoter
    {
        static char GuessPromotionSymbol(char symbol)
        {
            const char upped = static_cast<char>(std::toupper(symbol));
            if (upped == ::Chess::PieceTypeConverter::TryConvertToChar(::Chess::ePieceType::KING))
            {
                return ::Chess::PieceTypeConverter::TryConvertToChar(::Chess::ePieceType::KNIGHT).value();
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

    public:
        virtual ::Chess::ePieceType GetPromoteType() const override
        {
            while (true)
            {
                GetSignalOnEnter()(::Chess::eInputType::PROMOTION);

                const auto input      = EnterPromotionType();
                const auto normalized = TryNormalizePromotionChoice(input);
                if (normalized == ::Chess::PieceTypeConverter::TryConvertToChar(::Chess::ePieceType::BISHOP))
                {
                    return ::Chess::ePieceType::BISHOP;
                }
                if (normalized == ::Chess::PieceTypeConverter::TryConvertToChar(::Chess::ePieceType::KNIGHT))
                {
                    return ::Chess::ePieceType::KNIGHT;
                }
                if (normalized == ::Chess::PieceTypeConverter::TryConvertToChar(::Chess::ePieceType::QUEEN))
                {
                    return ::Chess::ePieceType::QUEEN;
                }
                if (normalized == ::Chess::PieceTypeConverter::TryConvertToChar(::Chess::ePieceType::ROOK))
                {
                    return ::Chess::ePieceType::ROOK;
                }
            }
        }
    };
} // namespace Console::Chess
