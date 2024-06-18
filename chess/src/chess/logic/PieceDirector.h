#pragma once

#include "Coordinate.h"
#include "../pieces/IPiece.h"

#include <boost/signals2.hpp>

#include <memory>
#include <vector>

namespace Chess
{
	class PieceDirector
	{
	private:
		std::vector<std::shared_ptr<IPiece>> m_eatenPieces{};
		std::shared_ptr<IPiece> m_currentPiece{};
		std::vector<std::shared_ptr<IPiece>> m_piecesOnBoard{};

	private:
		boost::signals2::signal<void()> m_moveSignal{};

	public:
		PieceDirector();

	public:
		const std::shared_ptr<IPiece>& get_CurrentPiece() const;
		const std::vector<std::shared_ptr<IPiece>>& get_EatenPieces() const;
		const std::vector<std::shared_ptr<IPiece>>& get_PiecesOnBoard() const;

	private:
		void Take(size_t indexOnBoard);

	public:
		PieceColorAndType GetPieceColorAndType(const Coordinate& from) const;
		std::shared_ptr<Chess::IPiece> GetPiece(const Coordinate& from) const;
		void InitCurrentPiece(const Coordinate& from);
		void MovePiece(const Coordinate& to);

	public:
		boost::signals2::connection ConnectMove(const boost::signals2::signal<void()>::slot_type& subscriber);
	};
}
