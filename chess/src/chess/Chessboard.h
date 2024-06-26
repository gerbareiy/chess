#pragma once

#include "logic/Coordinate.h"
#include "logic/MoveValidator.h"
#include "logic/PieceDirector.h"
#include "pieces/logic/PieceColorAndType.h"
#include "Player.h"

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
	class Chessboard
	{
	private:
		std::shared_ptr<PieceDirector> m_director;
		std::shared_ptr<MoveValidator> m_validator;
		std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard;

	private:
		boost::signals2::signal<void()> m_signalChessboardUndated;
		boost::signals2::signal<void()> m_signalInvalidInput;

	public:
		Chessboard();

	public:
		const std::shared_ptr<PieceDirector>& get_PieceDirector() const;
		const std::shared_ptr<MoveValidator>& get_MoveValidator() const;

	public:
		bool TryInitPiece(const Coordinate& from) const;
		
		//Before use this method, you need to InitPiece
		bool TryMovePiece(const Coordinate& to) const;

	public:
		boost::signals2::connection ConnectChessboardUndated(const boost::signals2::signal<void()>::slot_type& subscriber);
	};
}
