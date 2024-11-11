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
		Rook(ePieceColor pieceColor, int orderNumber, const std::shared_ptr<King>& king);
		Rook(ePieceColor color, Coordinate coordinate);
		Rook(ePieceColor color, Coordinate coordinate, const std::shared_ptr<King>& king);

	public:
		bool get_CanMakeCastling() const override;

	private:
		void DisableCastling();
		void MakeTracking(const std::shared_ptr<Chess::King>& king);
		void OnCastling(Coordinate to, eCastleSide side);

	public:
		void Move(Coordinate to, bool isRealMove = true) override;
	};
}
