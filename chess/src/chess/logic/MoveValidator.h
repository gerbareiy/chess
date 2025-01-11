#pragma once

#include <memory>
#include <vector>

namespace Chess
{
	class Piece;
	class CheckChecker;
	class MoveChecker;
	class Player;
	struct Coordinate;

	class MoveValidator
	{
	private:
		std::vector<std::shared_ptr<Piece>> m_piecesCanMove;
		std::vector<Coordinate> m_possibleMoves;
		const std::vector<std::shared_ptr<Piece>>& m_piecesOnBoard;
		const std::shared_ptr<Player> m_player;

	public:
		MoveValidator(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const std::shared_ptr<Player>& player);

	public:
		std::vector<Coordinate> GetPossibleMoves();

		void CalculatePiecesCanMove();
		void CalculatePossibleMoves(const std::shared_ptr<Piece>& piece);
		void ClearPossibleMoves();
		void ClearPiecesCanMove();
		int GetPiecesCanMoveCount();
		bool IsCoordinateInPieceCanMove(Coordinate coordinate) const;
		bool IsCoordinateInPossibleMoves(Coordinate coordinate) const;
		bool IsValidMove(const std::shared_ptr<Piece> piece, Coordinate to) const;
	};
}
