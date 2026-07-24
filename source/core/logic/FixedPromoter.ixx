export module Chess.Core.FixedPromoter;
import Chess.Core.ePieceType;
import Chess.Core.Promoter;

namespace Chess::Core
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
} // namespace Chess::Core
