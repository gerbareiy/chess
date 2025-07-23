module;
#include <boost/signals2.hpp>

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

                input[0] = std::toupper(input[0]);

                if (input[0] == PieceTypeConverter::ConvertToString(ePieceType::KING)[0])
                {
                    input[0] = std::tolower(input[0]);
                }

                if (input[0] == PieceTypeConverter::ConvertToString(ePieceType::BISHOP)[0])
                {
                    return ePieceType::BISHOP;
                }
                if (input[0] == PieceTypeConverter::ConvertToString(ePieceType::KNIGHT)[0])
                {
                    return ePieceType::KNIGHT;
                }
                if (input[0] == PieceTypeConverter::ConvertToString(ePieceType::QUEEN)[0])
                {
                    return ePieceType::QUEEN;
                }
                if (input[0] == PieceTypeConverter::ConvertToString(ePieceType::ROOK)[0])
                {
                    return ePieceType::ROOK;
                }
            }
        }
    };
} // namespace Chess
