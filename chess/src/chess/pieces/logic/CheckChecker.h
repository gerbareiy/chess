#pragma once

#include "ePieceColor.h"

#include <memory>
#include <vector>

namespace Chess
{
    class MoveChecker;
    class Piece;
    class PieceFinder;
    class King;

    class CheckChecker
    {
    public:
        static bool IsCheck(const std::shared_ptr<King>& king, const std::vector<std::shared_ptr<Piece>>& piece);
        static bool IsCheck(const ePieceColor& kingColor, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard);
    };
} // namespace Chess
