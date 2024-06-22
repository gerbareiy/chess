#include "../../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../../chess/src/chess/pieces/King.h"
#include "../../../../../chess/src/chess/pieces/logic/CheckChecker.h"
#include "../../../../../chess/src/chess/pieces/Rook.h"

#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(MoveCheckerTests)

BOOST_AUTO_TEST_CASE(Check_Test)
{
	auto blackRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, 1);
	auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE);
	auto whiteRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, 1);
	auto checker = std::make_shared<Chess::CheckChecker>();

	std::vector<std::shared_ptr<Chess::IPiece>> pieces = { blackRook, whiteRook, whiteKing };

	BOOST_CHECK(!checker->IsCheck(whiteKing, pieces));
	BOOST_CHECK(!checker->IsCheck(whiteRook->get_ColorAndType().get_Color(), pieces));

	blackRook->Move(Chess::Coordinate(whiteKing->get_Position().get_File(), whiteKing->get_Position().get_Rank() - 1));

	BOOST_CHECK(checker->IsCheck(whiteKing, pieces));
	BOOST_CHECK(checker->IsCheck(whiteRook->get_ColorAndType().get_Color(), pieces));
}

BOOST_AUTO_TEST_SUITE_END()