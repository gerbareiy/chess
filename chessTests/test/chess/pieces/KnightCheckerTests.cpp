#include "../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../chess/src/chess/pieces/Knight.h"
#include "../../../../chess/src/chess/pieces/KnightChecker.h"
#include "../../../../chess/src/chess/pieces/Pawn.h"
#include "../../../../chess/src/chess/pieces/logic/ePieceColor.h"
#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(MoveCheckerTests)

BOOST_AUTO_TEST_CASE(FindPosibleKnightMoves_Test)
{
    const auto blackKnight = std::make_shared<Chess::Knight>(Chess::ePieceColor::BLACK, 1);
    const auto blackPawn   = std::make_shared<Chess::Pawn>(Chess::ePieceColor::BLACK, 'B');
    const auto whitePawn1  = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, 'B');
    const auto whitePawn2  = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, 'B');

    const std::vector<std::shared_ptr<Chess::Piece>> pieces = { blackKnight, blackPawn, whitePawn1, whitePawn2 };

    const auto knightChecker = std::make_shared<Chess::KnightChecker>();

    BOOST_CHECK_EQUAL(knightChecker->GetMoves(blackPawn, pieces).size(), 0);

    blackKnight->Move(Chess::Coordinate('D', 1));
    blackPawn->Move(Chess::Coordinate('F', 2));
    whitePawn1->Move(Chess::Coordinate('E', 3));
    whitePawn2->Move(Chess::Coordinate('C', 3));
    BOOST_CHECK_EQUAL(knightChecker->GetMoves(blackKnight, pieces).size(), 3);
}

BOOST_AUTO_TEST_SUITE_END()
