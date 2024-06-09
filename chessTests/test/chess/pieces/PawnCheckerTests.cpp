#define BOOST_TEST_MODULE PawnCheckerTests

#include "../../../../chess/src/chess/pieces/ePieceColor.h"
#include "../../../../chess/src/chess/pieces/King.h"
#include "../../../../chess/src/chess/pieces/PawnChecker.h"
#include "../../../../chess/src/chess/pieces/Pawn.h"
#include "../../../../chess/src/chess/pieces/PawnChecker.h"
#include "../../../../chess/src/chess/pieces/Queen.h"

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(FindPosibleMoves_Test)
{
    auto pawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, 'B');
    auto king = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK);
    auto queen = std::make_shared<Chess::Queen>(Chess::ePieceColor::WHITE);

    std::vector<std::shared_ptr<Chess::IPiece>> pieces = { pawn, king, queen };
    auto pawnChecker = std::make_shared<Chess::PawnChecker>();

    BOOST_CHECK_THROW(pawnChecker->GetPossibleMoves(king, pieces), std::out_of_range);
    BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(pawn, pieces).size(), 2);

    king->Move(Chess::Coordinate('B', 3));
    BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(pawn, pieces).size(), 0);

    king->Move(Chess::Coordinate('C', 3));
    queen->Move(Chess::Coordinate('A', 3));
    BOOST_CHECK_EQUAL(pawnChecker->GetPossibleMoves(pawn, pieces).size(), 3);
}
