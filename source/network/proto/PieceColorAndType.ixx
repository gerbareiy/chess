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
        static chess::proto::PieceColor ToProto(Core::ePieceColor color)
        {
            switch (color)
            {
            case Core::ePieceColor::BLACK:
                return chess::proto::PIECE_COLOR_BLACK;
            case Core::ePieceColor::WHITE:
                return chess::proto::PIECE_COLOR_WHITE;
            default:
                return chess::proto::PIECE_COLOR_NONE;
            }
        }

        static Core::ePieceColor FromProto(chess::proto::PieceColor color)
        {
            switch (color)
            {
            case chess::proto::PIECE_COLOR_BLACK:
                return Core::ePieceColor::BLACK;
            case chess::proto::PIECE_COLOR_WHITE:
                return Core::ePieceColor::WHITE;
            default:
                return Core::ePieceColor::NONE;
            }
        }

        static chess::proto::PieceType ToProto(Core::ePieceType type)
        {
            switch (type)
            {
            case Core::ePieceType::BISHOP:
                return chess::proto::PIECE_TYPE_BISHOP;
            case Core::ePieceType::KING:
                return chess::proto::PIECE_TYPE_KING;
            case Core::ePieceType::KNIGHT:
                return chess::proto::PIECE_TYPE_KNIGHT;
            case Core::ePieceType::PAWN:
                return chess::proto::PIECE_TYPE_PAWN;
            case Core::ePieceType::QUEEN:
                return chess::proto::PIECE_TYPE_QUEEN;
            case Core::ePieceType::ROOK:
                return chess::proto::PIECE_TYPE_ROOK;
            default:
                return chess::proto::PIECE_TYPE_NONE;
            }
        }

        static Core::ePieceType FromProto(chess::proto::PieceType type)
        {
            switch (type)
            {
            case chess::proto::PIECE_TYPE_BISHOP:
                return Core::ePieceType::BISHOP;
            case chess::proto::PIECE_TYPE_KING:
                return Core::ePieceType::KING;
            case chess::proto::PIECE_TYPE_KNIGHT:
                return Core::ePieceType::KNIGHT;
            case chess::proto::PIECE_TYPE_PAWN:
                return Core::ePieceType::PAWN;
            case chess::proto::PIECE_TYPE_QUEEN:
                return Core::ePieceType::QUEEN;
            case chess::proto::PIECE_TYPE_ROOK:
                return Core::ePieceType::ROOK;
            default:
                return Core::ePieceType::NONE;
            }
        }

        static chess::proto::PieceColorAndType ToProto(const Core::PieceColorAndType& value)
        {
            chess::proto::PieceColorAndType result;
            result.set_color(ToProto(value.color));
            result.set_type(ToProto(value.type));
            return result;
        }

        static Core::PieceColorAndType FromProto(const chess::proto::PieceColorAndType& message)
        {
            return Core::PieceColorAndType{ FromProto(message.color()), FromProto(message.type()) };
        }
    };
} // namespace Chess::Network
