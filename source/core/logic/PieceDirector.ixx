module;
#include <functional>
#include <memory>
#include <typeinfo>
#include <vector>
export module Chess.Core.PieceDirector;
import Chess.Constants.Counts;
import Chess.Constants.Sizes;
import Chess.Core.CheckChecker;
import Chess.Core.Coordinate;
import Chess.Core.ePieceColor;
import Chess.Core.King;
import Chess.Core.Pawn;
import Chess.Core.Piece;
import Chess.Core.PieceColorAndType;
import Chess.Core.PieceTakeLocator;
import Chess.Core.Player;
import Chess.Core.Promoter;
import Chess.Core.Promotion;

namespace Chess::Core
{
    export class PieceDirector
    {
    public:
        PieceDirector(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player)
            : piecesOnBoard_(piecesOnBoard)
            , player_(player)
        {
            eatenPieces_.reserve(Constants::Counts::MAX_ELEMENTS_COUNT);
            promotion_ = std::make_shared<Promotion>();
        }

        const std::shared_ptr<Piece>& GetCurrentPiece() const
        {
            return currentPiece_;
        }

        const std::vector<std::shared_ptr<Piece>>& GetEatenPieces() const
        {
            return eatenPieces_;
        }

        bool GetIsCheck() const
        {
            return isCheck_;
        }

        const std::vector<std::shared_ptr<Piece>>& GetPiecesOnBoard() const
        {
            return piecesOnBoard_;
        }

        PieceColorAndType GetPieceColorAndType(const Coordinate& from) const
        {
            if (const auto piece = GetPiece(from))
            {
                return piece->GetColorAndType();
            }
            return {};
        }

        std::shared_ptr<Piece> GetPiece(const Coordinate& from) const
        {
            const auto iter = std::ranges::find(piecesOnBoard_, from, &Piece::GetPosition);
            return iter != piecesOnBoard_.end() ? *iter : nullptr;
        }

        void InitCurrentPiece(const Coordinate& from)
        {
            currentPiece_ = GetPiece(from);
        }

        void MovePiece(
            const Coordinate& to, const std::function<void()>& onChessboardUpdated, const std::shared_ptr<Promoter>& promoter)
        {
            const auto from = PieceTakeLocator::Find(currentPiece_, piecesOnBoard_, to);
            const auto iter = std::ranges::find(piecesOnBoard_, from, &Piece::GetPosition);
            if (iter != piecesOnBoard_.end())
            {
                Take(static_cast<int>(std::distance(piecesOnBoard_.begin(), iter)));
            }

            currentPiece_->Move(to);
            if (player_)
            {
                player_->InverseColor();
            }

            if (typeid(*currentPiece_) == typeid(Pawn)
                && (currentPiece_->GetPosition().rank == 1 && currentPiece_->GetColorAndType().color == ePieceColor::BLACK
                    || currentPiece_->GetPosition().rank == Constants::Sizes::CHESSBOARD_SIZE
                           && currentPiece_->GetColorAndType().color == ePieceColor::WHITE))
            {
                onChessboardUpdated();
                Promotion::PromoteConditionally(std::static_pointer_cast<Pawn>(currentPiece_), piecesOnBoard_, promoter);
            }

            auto color = ePieceColor::NONE;
            if (currentPiece_->GetColorAndType().color == ePieceColor::BLACK)
            {
                color = ePieceColor::WHITE;
            }
            else if (currentPiece_->GetColorAndType().color == ePieceColor::WHITE)
            {
                color = ePieceColor::BLACK;
            }

            isCheck_ = CheckChecker::IsCheck(color, piecesOnBoard_);
            if (const auto king = std::dynamic_pointer_cast<King>(currentPiece_))
            {
                king->SetCheck(isCheck_);
            }
        }

    private:
        std::shared_ptr<Piece>              currentPiece_;
        std::vector<std::shared_ptr<Piece>> eatenPieces_;
        bool                                isCheck_ = false;
        std::vector<std::shared_ptr<Piece>> piecesOnBoard_;
        std::shared_ptr<Promotion>          promotion_;
        std::shared_ptr<Player>             player_;

        void Take(int indexOnBoard)
        {
            eatenPieces_.emplace_back(std::move(piecesOnBoard_[indexOnBoard]));
            piecesOnBoard_.erase(piecesOnBoard_.begin() + indexOnBoard);
        }
    };
} // namespace Chess::Core
