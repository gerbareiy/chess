module;
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
export module Chess.Client.Session;
import Chess.Chessboard;
import Chess.ChessboardFactory;
import Chess.Client.eServerEvent;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.FixedPromoter;
import Chess.Move;
import Chess.Net.BoardSnapshot;
import Chess.Net.eSessionEvent;
import Chess.Net.GameSession;

namespace Chess::Client
{
    export class Session
    {
    public:
        static Session Connect(const std::string& host, unsigned short port)
        {
            auto session = Session(Net::GameSession::Connect(host, port));
            session.Init();
            return session;
        }

        ePieceColor GetMyColor() const
        {
            return myColor_;
        }

        const std::shared_ptr<Chessboard>& GetChessboard() const
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

        eGameState GetFinalState() const
        {
            return finalState_;
        }

        bool TrySubmitMove(const Move& move)
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
            case Net::eSessionEvent::OPPONENT_MOVED:
                TryApplyMove(move.value());
                return eServerEvent::OPPONENT_MOVED;
            case Net::eSessionEvent::BOARD_SYNCED:
                Rebuild(std::move(board).value());
                return eServerEvent::RESYNCED;
            case Net::eSessionEvent::BOARD_CHECKED:
                if (board.value().ply >= ply_)
                {
                    Rebuild(std::move(board).value());
                }
                return eServerEvent::RESYNCED;
            case Net::eSessionEvent::GAME_OVER:
                gameOver_   = true;
                finalState_ = finalState.value();
                Rebuild(std::move(board).value());
                return eServerEvent::GAME_OVER;
            default:
                return eServerEvent::RESYNCED;
            }
        }

    private:
        Net::GameSession            session_;
        std::shared_ptr<Chessboard> chessboard_;
        ePieceColor                 myColor_    = ePieceColor::NONE;
        eGameState                  finalState_ = eGameState::PLAYING;
        bool                        gameOver_   = false;
        uint32_t                    ply_        = 0;

        void Rebuild(Net::BoardSnapshot board)
        {
            ply_        = board.ply;
            chessboard_ = ChessboardFactory::Create(std::move(board.pieces), board.sideToMove);
        }

        bool TryApplyMove(const Move& move)
        {
            if (!chessboard_->TrySelectPiece(move.from))
            {
                return false;
            }
            if (!chessboard_->TryMovePiece(move.to, std::make_shared<FixedPromoter>(move.promotion)))
            {
                return false;
            }
            ++ply_;
            return true;
        }

        explicit Session(Net::GameSession session)
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
