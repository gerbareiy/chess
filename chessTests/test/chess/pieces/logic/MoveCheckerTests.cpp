#include "../../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../../chess/src/chess/pieces/Bishop.h"
#include "../../../../../chess/src/chess/pieces/King.h"
#include "../../../../../chess/src/chess/pieces/Rook.h"
#include "../../../../../chess/src/chess/pieces/logic/MoveChecker.h"
#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(MoveCheckerTests)

BOOST_AUTO_TEST_CASE(FilterMoveChecker_Test)
{
    const auto blackRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, 1);
    const auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE);
    auto       whiteRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, 1);
    const auto checker   = std::make_shared<Chess::MoveChecker>(whiteRook);

    std::vector<std::shared_ptr<Chess::Piece>> pieces = { blackRook, whiteRook, whiteKing };

    BOOST_CHECK_EQUAL(checker->GetFilteredMoves(pieces).size(), 10);

    blackRook->Move(Chess::Coordinate(whiteKing->GetPosition().GetFile() - 1, whiteKing->GetPosition().GetRank()));
    BOOST_CHECK_EQUAL(checker->GetFilteredMoves(pieces).size(), 1);

    blackRook->Move(Chess::Coordinate(whiteKing->GetPosition().GetFile(), whiteKing->GetPosition().GetRank() + 1));
    BOOST_CHECK_EQUAL(checker->GetFilteredMoves(pieces).size(), 0);

    auto blackBishop = std::make_shared<Chess::Bishop>(Chess::ePieceColor::BLACK, 1);
    pieces.emplace_back(blackBishop);

    blackBishop->Move(Chess::Coordinate(whiteKing->GetPosition().GetFile() + 2, whiteKing->GetPosition().GetRank() + 2));
    blackRook->Move(Chess::Coordinate(whiteKing->GetPosition().GetFile() - 1, whiteKing->GetPosition().GetRank()));
    BOOST_CHECK_EQUAL(checker->GetFilteredMoves(pieces).size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()