export module Chess.Promoter;
import Chess.eInputType;
import Chess.ePieceType;
import Chess.Inputer;

namespace Chess
{
    export class Promoter : public Inputer
    {
    public:
        virtual ~Promoter() = default;

        virtual ePieceType GetPromoteType() const = 0;
    };
} // namespace Chess
