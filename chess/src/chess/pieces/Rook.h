#pragma once

#include "logic/ePieceColor.h"
#include "logic/eCastleSide.h"
#include "logic/ICastlable.h"
#include "Piece.h"

#include <boost/signals2.hpp>

namespace Chess
{
	class King;
	struct Coordinate;

	class Rook : public Piece, public ICastable
	{
	private:
		bool m_canMakeCastling = false;
		boost::signals2::connection m_castlingConnection;

	public:
		Rook(ePieceColor pieceColor, int orderNumber);
		Rook(ePieceColor pieceColor, int orderNumber, std::shared_ptr<King> const& king);
		Rook(ePieceColor color, Coordinate const& coordinate);
		Rook(ePieceColor color, Coordinate coordinate, std::shared_ptr<King> const& king);

	public:
		bool GetCanMakeCastling() const override;

	private:
		void DisableCastling();
		void MakeTracking(std::shared_ptr<Chess::King> const& king);
		void OnCastling(Coordinate const& to, eCastleSide side);

	public:
		void Move(Coordinate to, bool isRealMove = true) override;
	};
}
