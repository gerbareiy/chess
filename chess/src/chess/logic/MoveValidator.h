#pragma once

#include "Coordinate.h"
#include "../pieces/IPiece.h"
#include "../pieces/logic/CheckChecker.h"
#include "../pieces/logic/MoveChecker.h"
#include "../Player.h"

#include <memory>
#include <vector>

namespace Chess
{
	class MoveValidator
	{
	private:
		std::vector<Coordinate> m_possibleMoves;
		std::shared_ptr<CheckChecker> m_checkChecker;
		std::vector<std::shared_ptr<IPiece>> m_piecesCanMove;
		const std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard;
		const std::shared_ptr<Player> m_player;

	public:
		MoveValidator(const std::vector<std::shared_ptr<IPiece>>& piecesOnBoard, const std::shared_ptr<Player>& player);

	public:
		std::vector<Coordinate> get_PossibleMoves();

	public:
		void CalculatePiecesCanMove();
		void CalculatePossibleMoves(const std::shared_ptr<IPiece>& piece);
		void ClearPossibleMoves();
		bool IsCoordinateInPieceCanMove(Coordinate coordinate);
		bool IsCoordinateInPossibleMoves(Coordinate coordinate);
		bool IsValidMove(const std::shared_ptr<IPiece> piece, Coordinate to);
	};
}
