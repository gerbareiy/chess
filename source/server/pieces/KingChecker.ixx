module;
#include <expected>
#include <memory>
#include <stdexcept>
#include <vector>
export module Chess.KingChecker;
import Chess.Coordinate;
import Chess.CoordinateToPieceBuilder;
import Chess.Counts;
import Chess.ePieceColor;
import Chess.Piece;
import Chess.PieceFinder;
import Chess.ICastable;
import Chess.IMoveChecker;
import Chess.King;
import Chess.Sizes;

namespace Chess
{
    export class KingChecker final : public IMoveChecker
    {
        static std::vector<Coordinate> FindCastlingMoves(const std::shared_ptr<const King>& king, const std::shared_ptr<PieceFinder>& finder)
        {
            std::vector<Coordinate> castlingMoves;

            if (!king->GetIsCheck() && king->GetCanMakeCastling())
            {
                const auto leftRook  = finder->Find({ .file = 'A', .rank = king->GetPosition().rank });
                const auto rightRook = finder->Find({ .file = 'A' + CHESSBOARD_SIZE - 1, .rank = king->GetPosition().rank });

                const auto canCastleLeft = leftRook && std::dynamic_pointer_cast<ICastable>(leftRook)
                                           && std::dynamic_pointer_cast<ICastable>(leftRook)->GetCanMakeCastling()
                                           && leftRook->GetColorAndType().color == king->GetColorAndType().color;

                const auto canCastleRight = rightRook && std::dynamic_pointer_cast<ICastable>(rightRook)
                                            && std::dynamic_pointer_cast<ICastable>(rightRook)->GetCanMakeCastling()
                                            && rightRook->GetColorAndType().color == king->GetColorAndType().color;

                if (canCastleLeft)
                {
                    auto pathClear = true;
                    for (char file = 'B'; file < king->GetPosition().file; ++file)
                    {
                        if (finder->Find({ .file = file, .rank = king->GetPosition().rank }))
                        {
                            pathClear = false;
                            break;
                        }
                    }

                    if (pathClear)
                    {
                        castlingMoves.emplace_back(king->GetPosition().file - 2, king->GetPosition().rank);
                    }
                }

                if (canCastleRight)
                {
                    auto pathClear = true;
                    for (char file = king->GetPosition().file + 1; file < 'A' + CHESSBOARD_SIZE - 1; ++file)
                    {
                        if (finder->Find({ .file = file, .rank = king->GetPosition().rank }))
                        {
                            pathClear = false;
                            break;
                        }
                    }

                    if (pathClear)
                    {
                        castlingMoves.emplace_back(king->GetPosition().file + 2, king->GetPosition().rank);
                    }
                }
            }

            return castlingMoves;
        }

        static std::expected<std::vector<Coordinate>, std::string> FindPossibleMoves(
            const std::shared_ptr<const King>& king, Coordinate position, ePieceColor color, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard)
        {
            if (position.file < 'A' || position.file > 'A' + CHESSBOARD_SIZE - 1 || position.rank < 1 || position.rank > CHESSBOARD_SIZE)
            {
                return std::unexpected("ChessPiece is out of the Chessboard");
            }

            std::vector<Coordinate> moves;
            moves.reserve(KING_WAYS_COUNT);

            auto       pieceMap = CoordinateToPieceBuilder::Build(piecesOnBoard);
            const auto finder   = std::make_shared<PieceFinder>(std::move(pieceMap));

            for (auto deltaFile = -1; deltaFile <= 1; ++deltaFile)
            {
                for (auto deltaRank = -1; deltaRank <= 1; ++deltaRank)
                {
                    if (deltaFile == 0 && deltaRank == 0)
                    {
                        continue;
                    }

                    char newFile = position.file + deltaFile;
                    int  newRank = position.rank + deltaRank;

                    const auto piece = finder->Find({ .file = newFile, .rank = newRank });

                    if (newFile >= 'A' && newFile <= 'A' + CHESSBOARD_SIZE - 1 && newRank >= 1 && newRank <= CHESSBOARD_SIZE
                        && (!piece || piece->GetColorAndType().color != color))
                    {
                        moves.emplace_back(newFile, newRank);
                    }
                }
            }

            auto castlingMoves = FindCastlingMoves(king, finder);
            moves.insert(moves.end(), castlingMoves.begin(), castlingMoves.end());

            return moves;
        }

    public:
        virtual std::vector<Coordinate> GetMoves(const std::shared_ptr<Piece>& piece, const std::vector<std::shared_ptr<Piece>>& piecesOnBoard) const override
        {
            if (const auto king = std::dynamic_pointer_cast<King>(piece))
            {
                auto possibleMoves = FindPossibleMoves(king, king->GetPosition(), king->GetColorAndType().color, piecesOnBoard);
                if (possibleMoves.has_value())
                {
                    return possibleMoves.value();
                }
                return {}; // TODO: somehow call checkmate here
            }
            return {};
        }
    };
} // namespace Chess
