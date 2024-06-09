#include "../../../../chess/src/chess/pieces/ePieceColor.h"
#include "../../../../chess/src/chess/pieces/Knight.h"
#include "../../../../chess/src/chess/pieces/KnightChecker.h"
#include "../../../../chess/src/chess/pieces/Pawn.h"

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(KnightTests)

BOOST_AUTO_TEST_CASE(FindPosibleKnightMoves_Test)
{
	auto blackKnight = std::make_shared<Chess::Knight>(Chess::ePieceColor::BLACK, 1);
	auto blackPawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, 'B');
	auto whitePawn1 = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, 'B');
	auto whitePawn2 = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, 'B');

	std::vector<std::shared_ptr<Chess::IPiece>> pieces = { blackKnight, blackPawn, whitePawn1, whitePawn2 };

	auto knightChecker = std::make_shared<Chess::KnightChecker>();

	BOOST_CHECK_THROW(knightChecker->GetPossibleMoves(blackPawn, pieces), std::out_of_range);

	blackKnight->Move(Chess::Coordinate('D', 1));
	blackPawn->Move(Chess::Coordinate('F', 2));
	whitePawn1->Move(Chess::Coordinate('E', 3));
	whitePawn2->Move(Chess::Coordinate('C', 3));
	BOOST_CHECK_EQUAL(knightChecker->GetPossibleMoves(blackKnight, pieces).size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()