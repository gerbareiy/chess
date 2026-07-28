module;
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <memory>
#include <vector>
export module Chess.Core.MoveValidator;
import Chess.Constants.Counts;
import Chess.Core.Coordinate;
import Chess.Core.CoordinateToPieceFactory;
import Chess.Core.MoveCheckerFactory;
import Chess.Core.MoveCheckerOwner;
import Chess.Core.Piece;
import Chess.Core.PieceFinder;
import Chess.Core.Player;

namespace Chess::Core
{
    export class MoveValidator
    {
    public:
        MoveValidator(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player)
            : piecesOnBoard_(piecesOnBoard)
            , player_(player)
        {
        }

        void Init()
        {
            RefreshPiecesCanMove();
        }

        std::vector<Coordinate> GetPossibleMoves()
        {
            return possibleMoves_;
        }

        void RefreshPiecesCanMove()
        {
            ClearPiecesCanMove();
            ClearPossibleMoves();

            std::vector<std::shared_ptr<Piece>> pieces;
            pieces.reserve(Constants::Counts::MAX_ELEMENTS_COUNT);

            for (const auto& piece : piecesOnBoard_)
            {
                if (piece->GetColorAndType().color == player_->GetPlayerColor())
                {
                    auto moveChecker = MoveCheckerFactory::Create(piece);
                    if (!moveChecker)
                    {
                        assert(false);
                        std::unreachable();
                    }

                    if (MoveCheckerOwner(piece, std::move(moveChecker)).HasFilteredMoves(piecesOnBoard_))
                    {
                        pieces.emplace_back(piece);
                    }
                }
            }

            piecesCanMove_ = pieces;
        }

        void RefreshPossibleMoves(const std::shared_ptr<Piece>& piece)
        {
            const auto iter = std::ranges::find(piecesCanMove_, piece);
            if (iter != piecesCanMove_.end())
            {
                auto moveChecker = MoveCheckerFactory::Create(piece);
                if (!moveChecker)
                {
                    assert(false);
                    std::unreachable();
                }
                possibleMoves_ = MoveCheckerOwner(piece, std::move(moveChecker)).GetFilteredMoves(piecesOnBoard_);
            }
        }

        void ClearPossibleMoves()
        {
            possibleMoves_.clear();
        }

        void ClearPiecesCanMove()
        {
            piecesCanMove_.clear();
        }

        size_t GetPiecesCanMoveCount() const
        {
            return piecesCanMove_.size();
        }

        bool IsCoordinateInPieceCanMove(const Coordinate& coordinate) const
        {
            auto       pieceMap = CoordinateToPieceFactory::Create(piecesCanMove_);
            const auto finder   = std::make_unique<PieceFinder>(std::move(pieceMap));
            return !!finder->TryFind(coordinate);
        }

        bool IsCoordinateInPossibleMoves(const Coordinate& coordinate) const
        {
            return std::ranges::contains(possibleMoves_, coordinate);
        }

        bool IsValidMove(const std::shared_ptr<Piece>& piece, const Coordinate& to) const
        {
            if (piece == nullptr)
            {
                return false;
            }
            return std::ranges::contains(possibleMoves_, to);
        }

    private:
        std::vector<std::shared_ptr<Piece>> piecesCanMove_;
        std::vector<Coordinate>             possibleMoves_;
        std::vector<std::shared_ptr<Piece>> piecesOnBoard_;
        std::shared_ptr<Player>             player_;
    };
} // namespace Chess::Core
