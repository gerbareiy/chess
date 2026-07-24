module;
#include <functional>
#include <memory>
#include <vector>
export module Chess.Core.Chessboard;
import Chess.Core.Coordinate;
import Chess.Utils.Event;
import Chess.Core.ePieceColor;
import Chess.Core.MoveValidator;
import Chess.Core.Piece;
import Chess.Core.PieceDirector;
import Chess.Core.Player;
import Chess.Core.Promoter;

namespace Chess::Core
{
    export class Chessboard
    {
    public:
        Utils::Event<void(), Chessboard> onChessboardUpdated;

        Chessboard(
            const std::shared_ptr<Player>&        player,
            std::vector<std::shared_ptr<Piece>>&& piecesOnBoard,
            std::unique_ptr<PieceDirector>&&      director,
            std::unique_ptr<MoveValidator>&&      validator)
            : player_(player)
            , piecesOnBoard_(std::move(piecesOnBoard))
            , director_(std::move(director))
            , validator_(std::move(validator))
        {
        }

        void Init()
        {
            player_->Init();
            validator_->Init();
        }

        Coordinate GetFrom() const
        {
            return from_;
        }

        ePieceColor GetSideToMove() const
        {
            return player_->GetPlayerColor();
        }

        const std::shared_ptr<MoveValidator>& GetMoveValidator() const
        {
            return validator_;
        }

        const std::shared_ptr<PieceDirector>& GetPieceDirector() const
        {
            return director_;
        }

        Coordinate GetTo() const
        {
            return to_;
        }

        bool TrySelectPiece(const Coordinate& from)
        {
            from_ = from;
            to_   = Coordinate(0, 0);
            director_->InitCurrentPiece(from);

            if (!director_->GetCurrentPiece())
            {
                return false;
            }

            validator_->RefreshPossibleMoves(director_->GetCurrentPiece());

            if (validator_->GetPossibleMoves().size() < 1)
            {
                return false;
            }

            onChessboardUpdated.Invoke();
            return true;
        }

        bool TryMovePiece(const Coordinate& to, const std::shared_ptr<Promoter>& promoter)
        {
            to_ = to;

            if (!validator_->IsValidMove(director_->GetCurrentPiece(), to))
            {
                return false;
            }

            validator_->ClearPossibleMoves();
            validator_->ClearPiecesCanMove();
            director_->MovePiece(to, [this] { onChessboardUpdated.Invoke(); }, promoter);
            validator_->RefreshPiecesCanMove();

            onChessboardUpdated.Invoke();

            return true;
        }

    private:
        std::shared_ptr<Player>             player_;
        std::vector<std::shared_ptr<Piece>> piecesOnBoard_;
        std::shared_ptr<PieceDirector>      director_;
        std::shared_ptr<MoveValidator>      validator_;

        Coordinate from_ = { .file = 0, .rank = 0 };
        Coordinate to_   = { .file = 0, .rank = 0 };
    };
} // namespace Chess::Core
