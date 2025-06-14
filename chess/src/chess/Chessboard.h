#pragma once

#include "logic/Coordinate.h"

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
	class MoveValidator;
	class Piece;
	class PieceDirector;
	class PieceSignalDirector;
	struct Coordinate;

	class Chessboard
	{
	private:
		std::shared_ptr<PieceDirector> m_director;
		Coordinate m_from = Coordinate(0, 0);
		std::shared_ptr<MoveValidator> m_validator;
		std::vector<std::shared_ptr<Piece>> m_piecesOnBoard;
		Coordinate m_to = Coordinate(0, 0);

	private:
		boost::signals2::signal<void()> m_signalChessboardUndated;

	public:
		Chessboard(std::vector<std::shared_ptr<Piece>> const& piecesOnBoard, std::shared_ptr<PieceSignalDirector> signalDirector);

	public:
		Coordinate GetFrom() const;
		std::shared_ptr<MoveValidator> const& GetMoveValidator() const;
		std::shared_ptr<PieceDirector> const& GetPieceDirector() const;
		Coordinate GetTo() const;

		bool TryInitPiece(Coordinate const& from);

		//Before use this method, you need to InitPiece
		bool TryMovePiece(Coordinate const& to);

	public:
		boost::signals2::connection ConnectChessboardUpdated(boost::signals2::signal<void()>::slot_type const& subscriber);
	};
}
