module;
#include <memory>
#include <vector>
export module Chess.PieceInitializer;
import Chess.Bishop;
import Chess.Counts;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.King;
import Chess.Knight;
import Chess.Pawn;
import Chess.Piece;
import Chess.Queen;
import Chess.Rook;
import Chess.Sizes;

namespace Chess
{
    export class PieceInitializer
    {
    public:
        static std::vector<std::shared_ptr<Piece>> InitNormalBoard()
        {
            std::vector<std::shared_ptr<Piece>> piecesOnBoard;
            piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);

            auto blackKing = std::make_shared<King>(ePieceColor::BLACK, Coordinate('E', CHESSBOARD_SIZE));
            auto whiteKing = std::make_shared<King>(ePieceColor::WHITE, Coordinate('E', 1));

            piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::BLACK, Coordinate('C', CHESSBOARD_SIZE)));
            piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::BLACK, Coordinate('F', CHESSBOARD_SIZE)));

            piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::WHITE, Coordinate('C', 1)));
            piecesOnBoard.push_back(std::make_shared<Bishop>(ePieceColor::WHITE, Coordinate('F', 1)));

            piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::BLACK, Coordinate('B', CHESSBOARD_SIZE)));
            piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::BLACK, Coordinate('G', CHESSBOARD_SIZE)));

            piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::WHITE, Coordinate('B', 1)));
            piecesOnBoard.push_back(std::make_shared<Knight>(ePieceColor::WHITE, Coordinate('G', 1)));

            piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::BLACK, Coordinate('A', CHESSBOARD_SIZE), blackKing));
            piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::BLACK, Coordinate('H', CHESSBOARD_SIZE), blackKing));

            piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::WHITE, Coordinate('A', 1), whiteKing));
            piecesOnBoard.push_back(std::make_shared<Rook>(ePieceColor::WHITE, Coordinate('H', 1), whiteKing));

            piecesOnBoard.push_back(blackKing);
            piecesOnBoard.push_back(whiteKing);

            for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
            {
                piecesOnBoard.push_back(std::make_shared<Pawn>(ePieceColor::BLACK, Coordinate('A' + i, CHESSBOARD_SIZE-1)));
                piecesOnBoard.push_back(std::make_shared<Pawn>(ePieceColor::WHITE, Coordinate('A' + i, 2)));
            }

            piecesOnBoard.push_back(std::make_shared<Queen>(ePieceColor::BLACK, Coordinate('D', CHESSBOARD_SIZE)));
            piecesOnBoard.push_back(std::make_shared<Queen>(ePieceColor::WHITE, Coordinate('D', 1)));

            return piecesOnBoard;
        }
    };
} // namespace Chess
