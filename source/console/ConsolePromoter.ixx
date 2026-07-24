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
    export class ConsolePromoter final : public Core::Promoter
    {
    public:
        virtual Core::ePieceType GetPromoteType() const override
        {
            while (true)
            {
                GetSignalOnEnter()(Core::eInputType::PROMOTION);

                const auto input      = EnterPromotionType();
                const auto normalized = TryNormalizePromotionChoice(input);
                if (normalized == Core::PieceTypeConverter::TryConvertToChar(Core::ePieceType::BISHOP))
                {
                    return Core::ePieceType::BISHOP;
                }
                if (normalized == Core::PieceTypeConverter::TryConvertToChar(Core::ePieceType::KNIGHT))
                {
                    return Core::ePieceType::KNIGHT;
                }
                if (normalized == Core::PieceTypeConverter::TryConvertToChar(Core::ePieceType::QUEEN))
                {
                    return Core::ePieceType::QUEEN;
                }
                if (normalized == Core::PieceTypeConverter::TryConvertToChar(Core::ePieceType::ROOK))
                {
                    return Core::ePieceType::ROOK;
                }
            }
        }

    private:
        static char GuessPromotionSymbol(char symbol)
        {
            const char upped = static_cast<char>(std::toupper(symbol));
            if (upped == Core::PieceTypeConverter::TryConvertToChar(Core::ePieceType::KING))
            {
                return Core::PieceTypeConverter::TryConvertToChar(Core::ePieceType::KNIGHT).value();
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
            auto result = Utils::ConsoleReader::ReadLine();
            std::getline(std::cin, result);
            return result;
        }
    };
} // namespace Chess::Console
