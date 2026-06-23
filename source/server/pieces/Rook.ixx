module;
#include <boost/signals2.hpp>
export module Chess.Rook;
import Chess.Coordinate;
import Chess.eCastleSide;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ICastable;
import Chess.King;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;
import Chess.Utils.Exceptions;

namespace Chess
{
    export class Rook final : public Piece, public ICastable
    {
        boost::signals2::scoped_connection m_connection;

        void TryMakeTracking(const std::shared_ptr<King>& king)
        {
            if (king == nullptr || !king->GetCanMakeCastling())
            {
                return;
            }
            std::optional<boost::signals2::scoped_connection> connection =
                king->TryConnectCastling(std::bind(&Rook::OnCastling, this, std::placeholders::_1, std::placeholders::_2));
            if (connection.has_value())
            {
                m_connection = std::move(connection).value();
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
                else if (GetPosition().file == 'A' + CHESSBOARD_SIZE - 1 && side == eCastleSide::RIGHT)
                {
                    Move(Coordinate(to.file - 1, to.rank));
                }
            }

            m_connection.disconnect();
        }

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
            return m_connection.connected();
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

            m_connection.disconnect();
            Piece::Move(to);
        }
    };
} // namespace Chess
