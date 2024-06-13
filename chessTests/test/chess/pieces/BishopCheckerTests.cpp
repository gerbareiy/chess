#include "../../../../chess/src/chess/Sizes.h"
#include "../../../../chess/src/chess/Coordinate.h"
#include "../../../../chess/src/chess/pieces/Bishop.h"
#include "../../../../chess/src/chess/pieces/BishopChecker.h"
#include "../../../../chess/src/chess/pieces/ePieceColor.h"
#include "../../../../chess/src/chess/pieces/Knight.h"

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(KnightTests)

BOOST_AUTO_TEST_CASE(FindPosibleBishopMoves_Test)
{
	auto blackBishop = std::make_shared<Chess::Bishop>(Chess::ePieceColor::BLACK, 1);
	auto whiteBishop = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, 2);
	auto whiteKnight = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, 1);

	std::vector<std::shared_ptr<Chess::IPiece>> pieces = { blackBishop, whiteBishop, whiteKnight };
	auto bishopChecker = std::make_shared<Chess::BishopChecker>();

	BOOST_CHECK_THROW(bishopChecker->GetPossibleMoves(whiteKnight, pieces), std::out_of_range);

	blackBishop->Move(Chess::Coordinate('G', 2));
	whiteBishop->Move(Chess::Coordinate('E', 4));
	whiteKnight->Move(Chess::Coordinate('G', 6));
	BOOST_CHECK_EQUAL(bishopChecker->GetPossibleMoves(whiteBishop, pieces).size(), 10);
}

BOOST_AUTO_TEST_SUITE_END()