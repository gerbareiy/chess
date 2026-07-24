module;
#include "PieceColorAndType.pb.h"
export module Chess.Network.PieceColorAndType;
import Chess.Core.ePieceColor;
import Chess.Core.ePieceType;
import Chess.Core.PieceColorAndType;

namespace Chess::Network
{
    export class PieceColorAndType
    {
    public:
        static chess::proto::PieceColor ToProto(Chess::Core::ePieceColor color)
        {
            switch (color)
            {
            case Chess::Core::ePieceColor::BLACK:
                return chess::proto::PIECE_COLOR_BLACK;
            case Chess::Core::ePieceColor::WHITE:
                return chess::proto::PIECE_COLOR_WHITE;
            default:
                return chess::proto::PIECE_COLOR_NONE;
            }
        }

        static Chess::Core::ePieceColor FromProto(chess::proto::PieceColor color)
        {
            switch (color)
            {
            case chess::proto::PIECE_COLOR_BLACK:
                return Chess::Core::ePieceColor::BLACK;
            case chess::proto::PIECE_COLOR_WHITE:
                return Chess::Core::ePieceColor::WHITE;
            default:
                return Chess::Core::ePieceColor::NONE;
            }
        }

        static chess::proto::PieceType ToProto(Chess::Core::ePieceType type)
        {
            switch (type)
            {
            case Chess::Core::ePieceType::BISHOP:
                return chess::proto::PIECE_TYPE_BISHOP;
            case Chess::Core::ePieceType::KING:
                return chess::proto::PIECE_TYPE_KING;
            case Chess::Core::ePieceType::KNIGHT:
                return chess::proto::PIECE_TYPE_KNIGHT;
            case Chess::Core::ePieceType::PAWN:
                return chess::proto::PIECE_TYPE_PAWN;
            case Chess::Core::ePieceType::QUEEN:
                return chess::proto::PIECE_TYPE_QUEEN;
            case Chess::Core::ePieceType::ROOK:
                return chess::proto::PIECE_TYPE_ROOK;
            default:
                return chess::proto::PIECE_TYPE_NONE;
            }
        }

        static Chess::Core::ePieceType FromProto(chess::proto::PieceType type)
        {
            switch (type)
            {
            case chess::proto::PIECE_TYPE_BISHOP:
                return Chess::Core::ePieceType::BISHOP;
            case chess::proto::PIECE_TYPE_KING:
                return Chess::Core::ePieceType::KING;
            case chess::proto::PIECE_TYPE_KNIGHT:
                return Chess::Core::ePieceType::KNIGHT;
            case chess::proto::PIECE_TYPE_PAWN:
                return Chess::Core::ePieceType::PAWN;
            case chess::proto::PIECE_TYPE_QUEEN:
                return Chess::Core::ePieceType::QUEEN;
            case chess::proto::PIECE_TYPE_ROOK:
                return Chess::Core::ePieceType::ROOK;
            default:
                return Chess::Core::ePieceType::NONE;
            }
        }

        static chess::proto::PieceColorAndType ToProto(const Chess::Core::PieceColorAndType& value)
        {
            chess::proto::PieceColorAndType result;
            result.set_color(ToProto(value.color));
            result.set_type(ToProto(value.type));
            return result;
        }

        static Chess::Core::PieceColorAndType FromProto(const chess::proto::PieceColorAndType& message)
        {
            return Chess::Core::PieceColorAndType{ FromProto(message.color()), FromProto(message.type()) };
        }
    };
} // namespace Chess::Network
