#pragma once

#include "ChessboardDisplayer.h"
#include "HandlerInputer.h"
#include "LabelShower.h"

#include <memory>

namespace Chess
{
    class Chessboard;
    class Controller;

    class Game
    {
    private:
        std::unique_ptr<ChessboardDisplayer> m_chessboardDisplayer;
        std::shared_ptr<Chessboard>          m_chessboard;
        std::shared_ptr<Controller>          m_controller;
        std::shared_ptr<HandlerInputer>      m_handlerInputer;
        std::unique_ptr<LabelShower>         m_inputDisplayer;

    public:
        Game();

    private:
        bool        ContinueGame() const;
        static void HandleInput(const std::function<Coordinate()>& inputFunction, const std::function<bool(const Coordinate&)>& initFunction);

    public:
        void Play() const;
    };
} // namespace Chess
