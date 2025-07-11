module;
#include <memory>
#include <vector>
export module Chess.PieceInitializer;
import Chess.Bishop;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.King;
import Chess.Knight;
import Chess.Pawn;
import Chess.Piece;
import Chess.PieceSignalDirector;
import Chess.Queen;
import Chess.Rook;
import Chess.Sizes;

namespace Chess
{
    export class PieceInitializer
    {
    public:
        static std::vector<std::shared_ptr<Piece>> InitNormalBoard(std::shared_ptr<PieceSignalDirector> signalDirector)
        {
            std::vector<std::shared_ptr<Piece>> piecesOnBoard;
            piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);

            auto blackKing = std::make_shared<King>(ePieceColor::BLACK, signalDirector);
            auto whiteKing = std::make_shared<King>(ePieceColor::WHITE, signalDirector);

            for (auto i = 1; i <= COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR; ++i)
            {
                piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::BLACK, i));
                piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::WHITE, i));
                piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::BLACK, i));
                piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::WHITE, i));
                piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::BLACK, i, blackKing));
                piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::WHITE, i, whiteKing));
            }

            piecesOnBoard.push_back(blackKing);
            piecesOnBoard.push_back(whiteKing);

            for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
            {
                piecesOnBoard.push_back(std::make_shared<Pawn>(ePieceColor::BLACK, 'A' + i, signalDirector));
                piecesOnBoard.push_back(std::make_shared<Pawn>(ePieceColor::WHITE, 'A' + i, signalDirector));
            }

            piecesOnBoard.push_back(std::make_shared<Queen>(ePieceColor::BLACK));
            piecesOnBoard.push_back(std::make_shared<Queen>(ePieceColor::WHITE));

            return piecesOnBoard;
        }
    };
} // namespace Chess
