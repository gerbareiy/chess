#include "../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../chess/src/chess/pieces/Bishop.h"
#include "../../../../chess/src/chess/pieces/Knight.h"
#include "../../../../chess/src/chess/pieces/Queen.h"
#include "../../../../chess/src/chess/pieces/QueenChecker.h"
#include "../../../../chess/src/chess/pieces/logic/ePieceColor.h"
#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(MoveCheckerTests)

BOOST_AUTO_TEST_CASE(FindPosibleQueenMoves_Test)
{
    const auto blackBishop = std::make_shared<Chess::Bishop>(Chess::ePieceColor::BLACK, 1);
    const auto whiteQueen  = std::make_shared<Chess::Queen>(Chess::ePieceColor::WHITE);
    const auto whiteKnight = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, 1);

    const std::vector<std::shared_ptr<Chess::Piece>> pieces        = { blackBishop, whiteQueen, whiteKnight };
    const auto                                       bishopChecker = std::make_shared<Chess::QueenChecker>();

    BOOST_CHECK_EQUAL(bishopChecker->GetMoves(whiteKnight, pieces).size(), 0);

    blackBishop->Move(Chess::Coordinate('G', 2));
    whiteQueen->Move(Chess::Coordinate('E', 4));
    whiteKnight->Move(Chess::Coordinate('G', 6));
    BOOST_CHECK_EQUAL(bishopChecker->GetMoves(whiteQueen, pieces).size(), 24);
}

BOOST_AUTO_TEST_SUITE_END()