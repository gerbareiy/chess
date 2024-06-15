#pragma once

#include "logic/Coordinate.h"
#include "logic/MoveValidator.h"
#include "logic/PieceDirector.h"
#include "pieces/logic/PieceColorAndType.h"

#include <memory>
#include <vector>

namespace Chess
{
	class Chessboard
	{
	private:
		std::shared_ptr<PieceDirector> m_director{};
		std::shared_ptr<MoveValidator> m_validator{};

	public:
		Chessboard();

	public:
		const std::shared_ptr<PieceDirector>& get_PieceDirector() const;
		const std::shared_ptr<MoveValidator>& get_MoveValidator() const;

	public:
		PieceColorAndType GetPieceColorAndType(const Coordinate& from) const;
		bool TryInitPiece(const Coordinate& from);
		bool TryMovePiece(const Coordinate& to);
	};
}
