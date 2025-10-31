module;
#include <boost/signals2.hpp>

#include <cctype>
#include <iostream>
#include <string>
export module Chess.ConsolePromoter;
import Chess.eInputType;
import Chess.ePieceType;
import Chess.Promoter;
import Chess.PieceTypeConverter;

namespace Chess
{
    export class ConsolePromoter final : public Promoter
    {
        static char NormalizePromotionChoice(const std::string& input)
        {
            for (unsigned char symbol : input)
            {
                if (std::isspace(symbol))
                {
                    continue;
                }

                auto normalized = static_cast<char>(std::toupper(static_cast<unsigned char>(symbol)));

                if (normalized == PieceTypeConverter::ConvertToShortString(ePieceType::KING)[0])
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
        virtual ePieceType GetPromoteType() const override
        {
            while (true)
            {
                m_signalOnEnter(eInputType::PROMOTION);

                std::string input;
                EnterPromotionType(input);

                const auto normalized = NormalizePromotionChoice(input);

                if (normalized == '\0')
                {
                    continue;
                }

                if (normalized == PieceTypeConverter::ConvertToShortString(ePieceType::BISHOP)[0])
                {
                    return ePieceType::BISHOP;
                }
                if (normalized == PieceTypeConverter::ConvertToShortString(ePieceType::KNIGHT)[0])
                {
                    return ePieceType::KNIGHT;
                }
                if (normalized == PieceTypeConverter::ConvertToShortString(ePieceType::QUEEN)[0])
                {
                    return ePieceType::QUEEN;
                }
                if (normalized == PieceTypeConverter::ConvertToShortString(ePieceType::ROOK)[0])
                {
                    return ePieceType::ROOK;
                }
            }
        }
    };
} // namespace Chess
