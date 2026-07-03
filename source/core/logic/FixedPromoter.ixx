export module Chess.FixedPromoter;
import Chess.ePieceType;
import Chess.Promoter;

namespace Chess
{
    // Неинтерактивный промоутер: возвращает заранее заданный тип превращения. Нужен там, где ход
    // применяется программно (сервер, приём чужого хода на клиенте), без запроса у пользователя.
    export class FixedPromoter final : public Promoter
    {
        ePieceType m_type;

    public:
        explicit FixedPromoter(ePieceType type = ePieceType::QUEEN)
            : m_type(type)
        {
        }

        virtual ePieceType GetPromoteType() const override
        {
            return m_type;
        }
    };
} // namespace Chess
