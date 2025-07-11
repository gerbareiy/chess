module;
#include <boost/signals2.hpp>
export module Chess.Rook;
import Chess.eCastleSide;
import Chess.eError;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.ErrorConverter;
import Chess.ICastable;
import Chess.King;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.Sizes;

namespace Chess
{
    export class Rook final : public Piece, public ICastable
    {
        bool                        m_canMakeCastling = false;
        boost::signals2::connection m_castlingConnection;

        void DisableCastling()
        {
            m_canMakeCastling = false;
        }

        void MakeTracking(const std::shared_ptr<King>& king)
        {
            if (!king)
            {
                return;
            }

            m_castlingConnection = king->ConnectCastling(std::bind(&Rook::OnCastling, this, std::placeholders::_1, std::placeholders::_2));
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

            m_castlingConnection.disconnect();
        }

    public:
        Rook(ePieceColor color, int orderNumber)
            : m_canMakeCastling(true)
        {
            if (orderNumber < 1 || orderNumber > 2)
            {
                throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_ONE_COLOR));
            }

            m_colorAndType = PieceColorAndType(color, ePieceType::ROOK);

            const auto file = orderNumber == 1 ? 'A' : 'A' + CHESSBOARD_SIZE - 1;

            switch (color)
            {
            case ePieceColor::BLACK:
                m_position = Coordinate(file, CHESSBOARD_SIZE);
                break;
            case ePieceColor::WHITE:
                m_position = Coordinate(file, 1);
                break;
            default:
                throw std::out_of_range(ErrorConverter::ToString(eError::OUT_OF_CHESSBOARD));
            }
        }

        Rook(ePieceColor pieceColor, int orderNumber, const std::shared_ptr<King>& king)
            : Rook(pieceColor, orderNumber)
        {
            MakeTracking(king);
        }

        Rook(ePieceColor color, const Coordinate& coordinate)
            : Piece(PieceColorAndType(color, ePieceType::ROOK), coordinate)
        {
        }

        Rook(ePieceColor color, Coordinate coordinate, const std::shared_ptr<King>& king)
            : Rook(color, coordinate)
        {
            MakeTracking(king);
        }

        virtual bool GetCanMakeCastling() const override
        {
            return m_canMakeCastling;
        }

        virtual void Move(Coordinate to, bool isRealMove = true) override
        {
            if (isRealMove)
            {
                DisableCastling();
            }

            Piece::Move(to);
        }
    };
} // namespace Chess
