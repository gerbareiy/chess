#pragma once

#include <memory>
#include <vector>

namespace Chess
{
    class Piece;
    class PieceSignalDirector;

    class PieceInitializer
    {
    public:
        static std::vector<std::shared_ptr<Piece>> InitNormalBoard(std::shared_ptr<PieceSignalDirector> signalDirector);
    };
} // namespace Chess
