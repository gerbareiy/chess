module;
#include "Chessboard.pb.h"

#include <memory>
#include <vector>
export module Chess.Proto.BoardReconstructor;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.King;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PieceFactory;
import Chess.Proto.Coordinate;
import Chess.Proto.PieceColorAndType;
import Chess.Rook;

namespace Chess::Proto
{
    // Перестраивает доменные фигуры из proto-доски, восстанавливая скрытое, не выводимое из позиции
    // состояние: право короля на рокировку, право конкретной ладьи на рокировку и состояние взятия
    // на проходе у пешки. Именно здесь лечится десинк «король ещё может рокировать, а эта ладья уже
    // нет» (и наоборот).
    export class BoardReconstructor
    {
        static std::shared_ptr<Chess::King> KingOf(
            Chess::ePieceColor color, const std::shared_ptr<Chess::King>& whiteKing, const std::shared_ptr<Chess::King>& blackKing)
        {
            return color == Chess::ePieceColor::WHITE ? whiteKing : blackKing;
        }

    public:
        static std::vector<std::shared_ptr<Chess::Piece>> Reconstruct(const chess::proto::Chessboard& board)
        {
            std::vector<std::shared_ptr<Chess::Piece>> result;
            result.reserve(board.pieces_size());

            std::shared_ptr<Chess::King> whiteKing;
            std::shared_ptr<Chess::King> blackKing;

            // 1) Короли — с сохранением их права на рокировку.
            for (const auto& piece : board.pieces())
            {
                const auto colorAndType = PieceColorAndType::FromProto(piece.color_and_type());
                if (colorAndType.type != Chess::ePieceType::KING)
                {
                    continue;
                }

                const auto coordinate = Coordinate::FromProto(piece.coordinate());
                auto       king       = std::make_shared<Chess::King>(colorAndType.color, coordinate, piece.king_can_castle());
                (colorAndType.color == Chess::ePieceColor::WHITE ? whiteKing : blackKing) = king;
                result.push_back(std::move(king));
            }

            // 2) Остальные фигуры.
            for (const auto& piece : board.pieces())
            {
                const auto colorAndType = PieceColorAndType::FromProto(piece.color_and_type());
                if (colorAndType.type == Chess::ePieceType::KING)
                {
                    continue;
                }

                const auto coordinate = Coordinate::FromProto(piece.coordinate());

                switch (colorAndType.type)
                {
                case Chess::ePieceType::ROOK:
                {
                    // Ладья подключается к королю (и получает право рокировки) только если она сама
                    // ещё может рокироваться. Иначе передаём nullptr — ладья без рокировки, даже
                    // когда король ещё может.
                    const auto king = piece.rook_can_castle() ? KingOf(colorAndType.color, whiteKing, blackKing) : nullptr;
                    result.push_back(std::make_shared<Chess::Rook>(colorAndType.color, coordinate, king));
                    break;
                }
                case Chess::ePieceType::PAWN:
                {
                    auto pawn = std::make_shared<Chess::Pawn>(colorAndType.color, coordinate);
                    pawn->RestoreState(piece.pawn_can_en_passant(), piece.pawn_is_not_moved());
                    result.push_back(std::move(pawn));
                    break;
                }
                default:
                    result.push_back(Chess::PieceFactory::Create(colorAndType, coordinate, nullptr));
                    break;
                }
            }

            return result;
        }
    };
} // namespace Chess::Proto
