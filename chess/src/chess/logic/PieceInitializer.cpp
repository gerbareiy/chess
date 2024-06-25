#include "PieceInitializer.h"

#include "Counts.h"
#include "Sizes.h"
#include "../pieces/Bishop.h"
#include "../pieces/King.h"
#include "../pieces/Knight.h"
#include "../pieces/Pawn.h"
#include "../pieces/Queen.h"
#include "../pieces/Rook.h"

std::vector<std::shared_ptr<Chess::IPiece>> Chess::PieceInitializer::InitStandartBoard(std::shared_ptr<PieceSignalDirector> signalDirector) const
{
	std::vector<std::shared_ptr<Chess::IPiece>> piecesOnBoard;
	piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);

	auto blackKing = std::make_shared<King>(ePieceColor::BLACK, signalDirector);
	auto whiteKing = std::make_shared<King>(ePieceColor::WHITE, signalDirector);

	for (auto i = 1; i <= COUNT_OF_BISHOP_KNIGHT_ROOK_WITH_1_COLOR; ++i)
	{
		piecesOnBoard.emplace_back(new Bishop(ePieceColor::BLACK, i));
		piecesOnBoard.emplace_back(new Bishop(ePieceColor::WHITE, i));
		piecesOnBoard.emplace_back(new Knight(ePieceColor::BLACK, i));
		piecesOnBoard.emplace_back(new Knight(ePieceColor::WHITE, i));
		piecesOnBoard.emplace_back(new Rook(ePieceColor::BLACK, i, blackKing));
		piecesOnBoard.emplace_back(new Rook(ePieceColor::WHITE, i, whiteKing));
	}

	piecesOnBoard.emplace_back(blackKing);
	piecesOnBoard.emplace_back(whiteKing);

	for (auto i = 0; i < CHESSBOARD_SIZE; ++i)
	{
		piecesOnBoard.emplace_back(new Pawn(ePieceColor::BLACK, 'A' + i, signalDirector));
		piecesOnBoard.emplace_back(new Pawn(ePieceColor::WHITE, 'A' + i, signalDirector));
	}

	piecesOnBoard.emplace_back(new Queen(ePieceColor::BLACK));
	piecesOnBoard.emplace_back(new Queen(ePieceColor::WHITE));

	return piecesOnBoard;
}
