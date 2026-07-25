module;
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
export module Chess.Network.Match;
import Chess.Core.Chessboard;
import Chess.Core.eGameState;
import Chess.Core.ePieceColor;
import Chess.Core.FixedPromoter;
import Chess.Core.GameStateChecker;
import Chess.Core.Move;

namespace Chess::Network
{
    // Owns everything about one running game: who plays which colour, the board, and
    // whose turn it is. Knows nothing about sockets or the wire format.
    export class Match
    {
    public:
        Match(std::string whiteId, std::string blackId, std::shared_ptr<Core::Chessboard> board)
            : whiteId_(std::move(whiteId))
            , blackId_(std::move(blackId))
            , board_(std::move(board))
        {
        }

        const std::string& GetWhiteId() const
        {
            return whiteId_;
        }

        const std::string& GetBlackId() const
        {
            return blackId_;
        }

        const std::shared_ptr<Core::Chessboard>& GetBoard() const
        {
            return board_;
        }

        uint32_t GetPly() const
        {
            return ply_;
        }

        Core::eGameState GetState() const
        {
            return state_;
        }

        const std::string& GetCurrentId() const
        {
            return whiteToMove_ ? whiteId_ : blackId_;
        }

        const std::string& GetOpponentId() const
        {
            return whiteToMove_ ? blackId_ : whiteId_;
        }

        bool HasPlayer(const std::string& identity) const
        {
            return identity == whiteId_ || identity == blackId_;
        }

        const std::string& GetOtherPlayerId(const std::string& identity) const
        {
            return identity == whiteId_ ? blackId_ : whiteId_;
        }

        Core::ePieceColor GetColorOf(const std::string& identity) const
        {
            return identity == whiteId_ ? Core::ePieceColor::WHITE : Core::ePieceColor::BLACK;
        }

        bool GetIsFinished() const
        {
            return state_ == Core::eGameState::CHECKMATE || state_ == Core::eGameState::DRAW;
        }

        // Applies the move if it is legal, advancing the turn and recomputing the game
        // state. Returns whether the move was accepted.
        bool TryApplyMove(const Core::Move& move)
        {
            if (!board_->TrySelectPiece(move.from) || !board_->TryMovePiece(move.to, std::make_shared<Core::FixedPromoter>(move.promotion)))
            {
                return false;
            }

            ++ply_;
            state_       = checker_.Calculate(board_);
            whiteToMove_ = !whiteToMove_;
            return true;
        }

    private:
        std::string                       whiteId_;
        std::string                       blackId_;
        std::shared_ptr<Core::Chessboard> board_;
        Core::GameStateChecker            checker_;
        Core::eGameState                  state_       = Core::eGameState::PLAYING;
        uint32_t                          ply_         = 0;
        bool                              whiteToMove_ = true;
    };
} // namespace Chess::Network
