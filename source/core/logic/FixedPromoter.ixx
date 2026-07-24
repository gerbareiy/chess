export module Chess.FixedPromoter;
import Chess.ePieceType;
import Chess.Promoter;

namespace Chess
{
    export class FixedPromoter final : public Promoter
    {
    public:
        explicit FixedPromoter(ePieceType type = ePieceType::QUEEN)
            : type_(type)
        {
        }

        virtual ePieceType GetPromoteType() const override
        {
            return type_;
        }

    private:
        ePieceType type_;
    };
} // namespace Chess
