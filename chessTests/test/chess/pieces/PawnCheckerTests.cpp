#include "../../../../chess/src/chess/Sizes.h"
#include "../../../../chess/src/chess/Coordinate.h"
#include "../../../../chess/src/chess/pieces/ePieceColor.h"
#include "../../../../chess/src/chess/pieces/King.h"
#include "../../../../chess/src/chess/pieces/Pawn.h"
#include "../../../../chess/src/chess/pieces/PawnChecker.h"
#include "../../../../chess/src/chess/pieces/Queen.h"

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(KnightTests)

BOOST_AUTO_TEST_CASE(FindPosiblePawnMoves_Test)
{
	auto blackKing = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK);
	auto blackPawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, 'D');
	auto whitePawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, 'B');
	auto whiteQueen = std::make_shared<Chess::Queen>(Chess::ePieceColor::WHITE);

	std::vector<std::shared_ptr<Chess::IPiece>> pieces = { blackKing, blackPawn, whitePawn, whiteQueen };
	auto pawnChecker = std::make_shared<Chess::PawnChecker>();

	BOOST_CHECK_THROW(pawnChecker->GetPossibleMoves(blackKing, pieces), std::out_of_range);
	BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(whitePawn, pieces).size(), 2);

	blackKing->Move(Chess::Coordinate('B', 3));
	BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(whitePawn, pieces).size(), 0);

	blackKing->Move(Chess::Coordinate('C', 3));
	whiteQueen->Move(Chess::Coordinate('A', 3));
	BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(whitePawn, pieces).size(), 3);

	whitePawn->Move(Chess::Coordinate('B', Chess::CHESSBOARD_SIZE - 1));
	BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(whitePawn, pieces).size(), 1);

	whitePawn->Move(Chess::Coordinate('B', Chess::CHESSBOARD_SIZE));
	BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(whitePawn, pieces).size(), 0);

	blackKing->Move(Chess::Coordinate('B', Chess::CHESSBOARD_SIZE - 3));
	whitePawn->Move(Chess::Coordinate('C', Chess::CHESSBOARD_SIZE - 3));
	blackPawn->Move(Chess::Coordinate('D', Chess::CHESSBOARD_SIZE - 3));
	BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(whitePawn, pieces).size(), 2);
}

BOOST_AUTO_TEST_SUITE_END()