export module Chess.FixedPromoter;
import Chess.ePieceType;
import Chess.Promoter;

namespace Chess
{
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
