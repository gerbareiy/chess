#include "../../../../chess/src/chess/logic/Sizes.h"
#include "../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../chess/src/chess/pieces/Bishop.h"
#include "../../../../chess/src/chess/pieces/logic/ePieceColor.h"
#include "../../../../chess/src/chess/pieces/Knight.h"
#include "../../../../chess/src/chess/pieces/Queen.h"
#include "../../../../chess/src/chess/pieces/QueenChecker.h"

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(MoveCheckerTests)

BOOST_AUTO_TEST_CASE(FindPosibleQueenMoves_Test)
{
	auto blackBishop = std::make_shared<Chess::Bishop>(Chess::ePieceColor::BLACK, 1);
	auto whiteQueen = std::make_shared<Chess::Queen>(Chess::ePieceColor::WHITE);
	auto whiteKnight = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, 1);

	std::vector<std::shared_ptr<Chess::IPiece>> pieces = { blackBishop, whiteQueen, whiteKnight };
	auto bishopChecker = std::make_shared<Chess::QueenChecker>();

	BOOST_CHECK_EQUAL(bishopChecker->GetPossibleMoves(whiteKnight, pieces).size(), 0);

	blackBishop->Move(Chess::Coordinate('G', 2));
	whiteQueen->Move(Chess::Coordinate('E', 4));
	whiteKnight->Move(Chess::Coordinate('G', 6));
	BOOST_CHECK_EQUAL(bishopChecker->GetPossibleMoves(whiteQueen, pieces).size(), 24);
}

BOOST_AUTO_TEST_SUITE_END()