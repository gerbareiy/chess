#include "../../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../../chess/src/chess/pieces/King.h"
#include "../../../../../chess/src/chess/pieces/logic/MoveChecker.h"
#include "../../../../../chess/src/chess/pieces/Rook.h"

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(MoveCheckerTests)

BOOST_AUTO_TEST_CASE(FilterMoveChecker_Test)
{
	auto blackRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, 1);
	auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE);
	auto whiteRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, 1);
	auto checker = std::make_shared<Chess::MoveChecker>(whiteRook);

	std::vector<std::shared_ptr<Chess::IPiece>> pieces = { blackRook, whiteRook, whiteKing };

	BOOST_CHECK_EQUAL(checker->GetPossibleMoves(pieces).size(), 10);

	blackRook->Move(Chess::Coordinate(whiteKing->get_Position().get_File(), whiteKing->get_Position().get_Rank() - 1));
	BOOST_CHECK_EQUAL(checker->GetPossibleMoves(pieces).size(), 1);

	blackRook->Move(Chess::Coordinate(whiteKing->get_Position().get_File() + 1, whiteKing->get_Position().get_Rank()));
	BOOST_CHECK_EQUAL(checker->GetPossibleMoves(pieces).size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()