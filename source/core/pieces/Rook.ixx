module;
#include <boost/signals2.hpp>
export module Chess.Core.Rook;
import Chess.Constants.Sizes;
import Chess.Core.Coordinate;
import Chess.Core.eCastleSide;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.ICastable;
import Chess.Core.King;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Utils.Exceptions;

namespace Chess::Core
{
    export class Rook final : public Piece, public ICastable
    {
    public:
        Rook(ePieceColor color, Coordinate coordinate, const std::shared_ptr<King>& king = nullptr)
            : Piece(color, coordinate)
        {
            TryMakeTracking(king);
        }

        virtual PieceColorAndType GetColorAndType() const override
        {
            return { GetColor(), ePieceType::ROOK };
        }

        virtual bool GetCanMakeCastling() const override
        {
            return connection_.connected();
        }

        virtual void Move(Coordinate to) override
        {
            if (to.file != GetPosition().file && to.rank != GetPosition().rank)
            {
                throw Utils::ImpossibleMoveException();
            }
            if (to.file == GetPosition().file && to.rank == GetPosition().rank)
            {
                throw Utils::ImpossibleMoveException();
            }

            connection_.disconnect();
            Piece::Move(to);
        }

    private:
        boost::signals2::scoped_connection connection_;

        void TryMakeTracking(const std::shared_ptr<King>& king)
        {
            if (king == nullptr || !king->GetCanMakeCastling())
            {
                return;
            }
            std::optional<boost::signals2::scoped_connection> connection =
                king->TryConnectOnCastling(std::bind(&Rook::OnCastling, this, std::placeholders::_1, std::placeholders::_2));
            if (connection.has_value())
            {
                connection_ = std::move(connection).value();
            }
        }

        void OnCastling(const Coordinate& to, eCastleSide side)
        {
            if (GetCanMakeCastling())
            {
                if (GetPosition().file == 'A' && side == eCastleSide::LEFT)
                {
                    Move(Coordinate(to.file + 1, to.rank));
                }
                else if (GetPosition().file == 'A' + Constants::Sizes::CHESSBOARD_SIZE - 1 && side == eCastleSide::RIGHT)
                {
                    Move(Coordinate(to.file - 1, to.rank));
                }
            }

            connection_.disconnect();
        }
    };
} // namespace Chess::Core
