export module Chess.Core.Promoter;
import Chess.Core.eInputType;
import Chess.Core.ePieceType;
import Chess.Core.Inputter;

namespace Chess::Core
{
    export class Promoter : public Inputter
    {
    public:
        virtual ~Promoter() override = default;

        virtual ePieceType GetPromoteType() const = 0;
    };
} // namespace Chess::Core
