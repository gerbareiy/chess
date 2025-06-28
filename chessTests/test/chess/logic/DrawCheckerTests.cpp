#include "../../../../chess/src/chess/Chessboard.h"
#include "../../../../chess/src/chess/logic/Coordinate.h"
#include "../../../../chess/src/chess/logic/DrawChecker.h"
#include "../../../../chess/src/chess/logic/PieceSignalDirector.h"
#include "../../../../chess/src/chess/pieces/Bishop.h"
#include "../../../../chess/src/chess/pieces/King.h"
#include "../../../../chess/src/chess/pieces/Knight.h"
#include "../../../../chess/src/chess/pieces/Pawn.h"
#include "../../../../chess/src/chess/pieces/Rook.h"
#include <boost/test/unit_test.hpp>

#include <memory>

BOOST_AUTO_TEST_SUITE(DrawCheckerTests)

BOOST_AUTO_TEST_CASE(IsInsufficientMaterial_Test)
{
    auto signalDirector = std::make_shared<Chess::PieceSignalDirector>();
    auto drawChecker    = std::make_shared<Chess::DrawChecker>();

    auto                                       blackKing1   = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 5));
    auto                                       whitePawn1   = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 1));
    auto                                       whiteKing1   = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 3));
    auto                                       whiteKnight1 = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 2));
    std::vector<std::shared_ptr<Chess::Piece>> pieces1      = { blackKing1, whitePawn1, whiteKing1, whiteKnight1 };
    auto                                       chessboard1  = std::make_shared<Chess::Chessboard>(pieces1, signalDirector);

    auto                                       blackKing2   = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 5));
    auto                                       whiteKing2   = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 3));
    auto                                       whiteKnight2 = std::make_shared<Chess::Knight>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 2));
    std::vector<std::shared_ptr<Chess::Piece>> pieces2      = { blackKing2, whiteKing2, whiteKnight2 };
    auto                                       chessboard2  = std::make_shared<Chess::Chessboard>(pieces2, signalDirector);

    auto                                       blackKing3   = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 5));
    auto                                       whiteKing3   = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 3));
    auto                                       whiteBishop3 = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 2));
    std::vector<std::shared_ptr<Chess::Piece>> pieces3      = { blackKing3, whiteBishop3, whiteKing3 };
    auto                                       chessboard3  = std::make_shared<Chess::Chessboard>(pieces3, signalDirector);

    auto                                       blackKing4     = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 5));
    auto                                       whiteKing4     = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 3));
    auto                                       whiteBishop4   = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 2));
    auto                                       whiteBishop4_1 = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate('B', 1));
    std::vector<std::shared_ptr<Chess::Piece>> pieces4        = { blackKing4, whiteBishop4, whiteBishop4_1, whiteKing4 };
    auto                                       chessboard4    = std::make_shared<Chess::Chessboard>(pieces4, signalDirector);

    auto                                       blackKing5     = std::make_shared<Chess::King>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 5));
    auto                                       whiteKing5     = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 3));
    auto                                       whiteBishop5   = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 2));
    auto                                       whiteBishop5_1 = std::make_shared<Chess::Bishop>(Chess::ePieceColor::WHITE, Chess::Coordinate('B', 2));
    std::vector<std::shared_ptr<Chess::Piece>> pieces5        = { blackKing5, whiteBishop5, whiteBishop5_1, whiteKing5 };
    auto                                       chessboard5    = std::make_shared<Chess::Chessboard>(pieces5, signalDirector);

    BOOST_CHECK(!drawChecker->IsDraw(chessboard1));
    BOOST_CHECK(drawChecker->IsDraw(chessboard2));
    BOOST_CHECK(drawChecker->IsDraw(chessboard3));
    BOOST_CHECK(drawChecker->IsDraw(chessboard4));
    BOOST_CHECK(!drawChecker->IsDraw(chessboard5));
}

BOOST_AUTO_TEST_CASE(IsDontHaveMoves_Test)
{
    auto       signalDirector = std::make_shared<Chess::PieceSignalDirector>();
    const auto drawChecker    = std::make_shared<Chess::DrawChecker>();

    const auto                                 blackKing  = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 1));
    const auto                                 blackRook1 = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('C', 4));
    const auto                                 blackRook2 = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('C', 6));
    const auto                                 blackRook3 = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('E', 4));
    const auto                                 blackRook4 = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('E', 6));
    const auto                                 whiteKing  = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('D', 5));
    std::vector<std::shared_ptr<Chess::Piece>> pieces     = { blackKing, blackRook1, blackRook2, blackRook3, blackRook4, whiteKing };

    const auto chessboard = std::make_shared<Chess::Chessboard>(pieces, signalDirector);

    BOOST_CHECK(drawChecker->IsDraw(chessboard));
}

BOOST_AUTO_TEST_CASE(SeventyFiveMovesLogic_Test)
{
    auto       signalDirector = std::make_shared<Chess::PieceSignalDirector>();
    const auto drawChecker    = std::make_shared<Chess::DrawChecker>();

    const auto blackKing = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('A', 1));
    const auto blackRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::BLACK, Chess::Coordinate('C', 6));
    const auto whiteKing = std::make_shared<Chess::King>(Chess::ePieceColor::WHITE, Chess::Coordinate('D', 5));
    const auto whitePawn = std::make_shared<Chess::Pawn>(Chess::ePieceColor::WHITE, Chess::Coordinate('A', 2));
    const auto whiteRook = std::make_shared<Chess::Rook>(Chess::ePieceColor::WHITE, Chess::Coordinate('C', 4));

    std::vector<std::shared_ptr<Chess::Piece>> pieces     = { blackKing, blackRook, whiteKing, whitePawn, whiteRook };
    const auto                                 chessboard = std::make_shared<Chess::Chessboard>(pieces, signalDirector);

    BOOST_CHECK(!drawChecker->IsDraw(chessboard));

    chessboard->TryInitPiece(Chess::Coordinate('C', 4));
    chessboard->TryMovePiece(Chess::Coordinate('C', 3));
    chessboard->TryInitPiece(Chess::Coordinate('C', 6));
    chessboard->TryMovePiece(Chess::Coordinate('C', 7));

    for (int i = 0; i < 80; i++)
    {
        drawChecker->IsDraw(chessboard);
    }

    BOOST_CHECK(drawChecker->IsDraw(chessboard));

    chessboard->TryInitPiece(Chess::Coordinate('A', 2));
    chessboard->TryMovePiece(Chess::Coordinate('A', 3));

    BOOST_CHECK(!drawChecker->IsDraw(chessboard));

    chessboard->TryInitPiece(Chess::Coordinate('C', 7));
    chessboard->TryMovePiece(Chess::Coordinate('C', 6));

    for (int i = 0; i < 80; i++)
    {
        drawChecker->IsDraw(chessboard);
    }

    BOOST_CHECK(drawChecker->IsDraw(chessboard));

    chessboard->TryInitPiece(Chess::Coordinate('C', 3));
    chessboard->TryMovePiece(Chess::Coordinate('C', 6));

    BOOST_CHECK(!drawChecker->IsDraw(chessboard));
}

BOOST_AUTO_TEST_SUITE_END()
