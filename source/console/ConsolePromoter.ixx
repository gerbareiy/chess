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

namespace Console::Chess
{
    export class ConsolePromoter final : public ::Chess::Promoter
    {
        static char NormalizePromotionChoice(const std::string& input)
        {
            for (const unsigned char symbol : input)
            {
                if (std::isspace(symbol))
                {
                    continue;
                }

                auto normalized = static_cast<char>(std::toupper(symbol));

                if (normalized == ::Chess::PieceTypeConverter::ConvertToShortString(::Chess::ePieceType::KING)[0])
                {
                    normalized = static_cast<char>(std::tolower(static_cast<unsigned char>(normalized)));
                }

                return normalized;
            }

            return '\0';
        }

        static void EnterPromotionType(std::string& input)
        {
            std::getline(std::cin, input);
        }

    public:
        virtual ::Chess::ePieceType GetPromoteType() const override
        {
            while (true)
            {
                GetSignalOnEnter()(::Chess::eInputType::PROMOTION);

                std::string input;
                EnterPromotionType(input);

                const auto normalized = NormalizePromotionChoice(input);

                if (normalized == '\0')
                {
                    continue;
                }

                if (normalized == ::Chess::PieceTypeConverter::ConvertToShortString(::Chess::ePieceType::BISHOP)[0])
                {
                    return ::Chess::ePieceType::BISHOP;
                }
                if (normalized == ::Chess::PieceTypeConverter::ConvertToShortString(::Chess::ePieceType::KNIGHT)[0])
                {
                    return ::Chess::ePieceType::KNIGHT;
                }
                if (normalized == ::Chess::PieceTypeConverter::ConvertToShortString(::Chess::ePieceType::QUEEN)[0])
                {
                    return ::Chess::ePieceType::QUEEN;
                }
                if (normalized == ::Chess::PieceTypeConverter::ConvertToShortString(::Chess::ePieceType::ROOK)[0])
                {
                    return ::Chess::ePieceType::ROOK;
                }
            }
        }
    };
} // namespace Console::Chess
