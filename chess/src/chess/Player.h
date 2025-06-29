#pragma once

#include "pieces/logic/ePieceColor.h"

#include <memory>

namespace Chess
{
    class PieceSignalDirector;

    class Player
    {
    private:
        ePieceColor m_playerColor;

        void ChangeColor();

    public:
        Player(ePieceColor firstMoveColor, const std::shared_ptr<PieceSignalDirector>& signalDirector);

    public:
        ePieceColor GetPlayerColor();
    };
} // namespace Chess
