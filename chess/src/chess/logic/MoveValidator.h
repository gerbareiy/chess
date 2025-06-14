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
		std::vector<std::shared_ptr<Piece>> const& m_piecesOnBoard;
		std::shared_ptr<Player> const m_player;

	public:
		MoveValidator(std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<Player> const& player);

	public:
		std::vector<Coordinate> GetPossibleMoves();

		void CalculatePiecesCanMove();
		void CalculatePossibleMoves(std::shared_ptr<Piece> const& piece);
		void ClearPossibleMoves();
		void ClearPiecesCanMove();
		int GetPiecesCanMoveCount();
		bool IsCoordinateInPieceCanMove(Coordinate const& coordinate) const;
		bool IsCoordinateInPossibleMoves(Coordinate const& coordinate) const;
		bool IsValidMove(std::shared_ptr<Piece> const& piece, Coordinate const& to) const;
	};
}
