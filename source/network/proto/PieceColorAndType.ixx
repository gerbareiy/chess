module;
#include "PieceColorAndType.pb.h"
export module Chess.Proto.PieceColorAndType;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.PieceColorAndType;

namespace Chess::Proto
{
    export class PieceColorAndType
    {
    public:
        static chess::proto::PieceColor ToProto(Chess::ePieceColor color)
        {
            switch (color)
            {
            case Chess::ePieceColor::BLACK:
                return chess::proto::PIECE_COLOR_BLACK;
            case Chess::ePieceColor::WHITE:
                return chess::proto::PIECE_COLOR_WHITE;
            default:
                return chess::proto::PIECE_COLOR_NONE;
            }
        }

        static Chess::ePieceColor FromProto(chess::proto::PieceColor color)
        {
            switch (color)
            {
            case chess::proto::PIECE_COLOR_BLACK:
                return Chess::ePieceColor::BLACK;
            case chess::proto::PIECE_COLOR_WHITE:
                return Chess::ePieceColor::WHITE;
            default:
                return Chess::ePieceColor::NONE;
            }
        }

        static chess::proto::PieceType ToProto(Chess::ePieceType type)
        {
            switch (type)
            {
            case Chess::ePieceType::BISHOP:
                return chess::proto::PIECE_TYPE_BISHOP;
            case Chess::ePieceType::KING:
                return chess::proto::PIECE_TYPE_KING;
            case Chess::ePieceType::KNIGHT:
                return chess::proto::PIECE_TYPE_KNIGHT;
            case Chess::ePieceType::PAWN:
                return chess::proto::PIECE_TYPE_PAWN;
            case Chess::ePieceType::QUEEN:
                return chess::proto::PIECE_TYPE_QUEEN;
            case Chess::ePieceType::ROOK:
                return chess::proto::PIECE_TYPE_ROOK;
            default:
                return chess::proto::PIECE_TYPE_NONE;
            }
        }

        static Chess::ePieceType FromProto(chess::proto::PieceType type)
        {
            switch (type)
            {
            case chess::proto::PIECE_TYPE_BISHOP:
                return Chess::ePieceType::BISHOP;
            case chess::proto::PIECE_TYPE_KING:
                return Chess::ePieceType::KING;
            case chess::proto::PIECE_TYPE_KNIGHT:
                return Chess::ePieceType::KNIGHT;
            case chess::proto::PIECE_TYPE_PAWN:
                return Chess::ePieceType::PAWN;
            case chess::proto::PIECE_TYPE_QUEEN:
                return Chess::ePieceType::QUEEN;
            case chess::proto::PIECE_TYPE_ROOK:
                return Chess::ePieceType::ROOK;
            default:
                return Chess::ePieceType::NONE;
            }
        }

        static chess::proto::PieceColorAndType ToProto(const Chess::PieceColorAndType& value)
        {
            chess::proto::PieceColorAndType result;
            result.set_color(ToProto(value.color));
            result.set_type(ToProto(value.type));
            return result;
        }

        static Chess::PieceColorAndType FromProto(const chess::proto::PieceColorAndType& message)
        {
            return Chess::PieceColorAndType{ FromProto(message.color()), FromProto(message.type()) };
        }
    };
} // namespace Chess::Proto
