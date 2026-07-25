module;
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
export module Chess.Client.Session;
import Chess.Client.eServerEvent;
import Chess.Core.Chessboard;
import Chess.Core.ChessboardFactory;
import Chess.Core.eGameState;
import Chess.Core.ePieceColor;
import Chess.Core.FixedPromoter;
import Chess.Core.Move;
import Chess.Network.BoardSnapshot;
import Chess.Network.eSessionEvent;
import Chess.Network.GameSession;

namespace Chess::Client
{
    export class Session
    {
    public:
        static Session Connect(const std::string& host, unsigned short port)
        {
            auto session = Session(Network::GameSession::Connect(host, port));
            session.Init();
            return session;
        }

        Core::ePieceColor GetMyColor() const
        {
            return myColor_;
        }

        const std::shared_ptr<Core::Chessboard>& GetChessboard() const
        {
            return chessboard_;
        }

        bool IsMyTurn() const
        {
            return !IsGameOver() && chessboard_->GetSideToMove() == myColor_;
        }

        bool IsGameOver() const
        {
            return gameOver_;
        }

        Core::eGameState GetFinalState() const
        {
            return finalState_;
        }

        bool GetOpponentLeft() const
        {
            return opponentLeft_;
        }

        bool TrySubmitMove(const Core::Move& move)
        {
            if (IsMyTurn() && TryApplyMove(move))
            {
                session_.SendMove(move);
                return true;
            }
            return false;
        }

        eServerEvent ReceiveNext()
        {
            auto [event, move, board, finalState] = session_.ReceiveNext();

            switch (event)
            {
            case Network::eSessionEvent::OPPONENT_MOVED:
                TryApplyMove(move.value());
                return eServerEvent::OPPONENT_MOVED;
            case Network::eSessionEvent::BOARD_SYNCED:
                Rebuild(std::move(board).value());
                return eServerEvent::RESYNCED;
            case Network::eSessionEvent::BOARD_CHECKED:
                if (board.value().ply >= ply_)
                {
                    Rebuild(std::move(board).value());
                }
                return eServerEvent::RESYNCED;
            case Network::eSessionEvent::GAME_OVER:
                gameOver_   = true;
                finalState_ = finalState.value();
                Rebuild(std::move(board).value());
                return eServerEvent::GAME_OVER;
            case Network::eSessionEvent::OPPONENT_LEFT:
                gameOver_     = true;
                opponentLeft_ = true;
                return eServerEvent::OPPONENT_LEFT;
            default:
                return eServerEvent::RESYNCED;
            }
        }

    private:
        Network::GameSession              session_;
        std::shared_ptr<Core::Chessboard> chessboard_;
        Core::ePieceColor                 myColor_      = Core::ePieceColor::NONE;
        Core::eGameState                  finalState_   = Core::eGameState::PLAYING;
        bool                              gameOver_     = false;
        bool                              opponentLeft_ = false;
        uint32_t                          ply_          = 0;

        void Rebuild(Network::BoardSnapshot board)
        {
            ply_        = board.ply;
            chessboard_ = Core::ChessboardFactory::Create(std::move(board.pieces), board.sideToMove);
        }

        bool TryApplyMove(const Core::Move& move)
        {
            if (!chessboard_->TrySelectPiece(move.from))
            {
                return false;
            }
            if (!chessboard_->TryMovePiece(move.to, std::make_shared<Core::FixedPromoter>(move.promotion)))
            {
                return false;
            }
            ++ply_;
            return true;
        }

        explicit Session(Network::GameSession session)
            : session_(std::move(session))
        {
        }

        void Init()
        {
            myColor_ = session_.GetMyColor();
            Rebuild(session_.GetInitialBoard());
        }
    };
} // namespace Chess::Client
