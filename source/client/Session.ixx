module;
#include "Envelope.pb.h"

#include <memory>
#include <string>
#include <utility>
export module Chess.Client.Session;
import Chess.Chessboard;
import Chess.ChessboardFactory;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.FixedPromoter;
import Chess.Move;
import Chess.Net.ClientConnection;
import Chess.Proto.Chessboard;
import Chess.Proto.Move;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Session;

namespace Chess::Client
{
    export enum class eServerEvent
    {
        OpponentMoved,
        Resynced,
        GameOver
    };

    export class Session
    {
        Net::ClientConnection       m_connection;
        std::shared_ptr<Chessboard> m_chessboard;
        ePieceColor                 m_myColor    = ePieceColor::NONE;
        bool                        m_gameOver   = false;
        eGameState                  m_finalState = eGameState::PLAYING;
        uint32_t                    m_ply        = 0;

        void Rebuild(const chess::proto::Chessboard& board)
        {
            auto       pieces = Proto::Chessboard::FromProto(board);
            const auto side   = Proto::PieceColorAndType::FromProto(board.side_to_move());
            m_chessboard      = ChessboardFactory::Create(std::move(pieces), side);
            m_ply             = board.ply();
        }

        bool ApplyMove(const Move& move)
        {
            if (!m_chessboard->TrySelectPiece(move.from))
            {
                return false;
            }
            if (!m_chessboard->TryMovePiece(move.to, std::make_shared<FixedPromoter>(move.promotion)))
            {
                return false;
            }
            ++m_ply;
            return true;
        }

        Session(const std::string& host, unsigned short port)
            : m_connection(Net::ClientConnection::Connect(host, port))
        {
            chess::proto::Envelope findGame;
            findGame.mutable_find_game();
            m_connection.SendBytes(findGame.SerializeAsString());

            chess::proto::Envelope started;
            started.ParseFromString(m_connection.ReceiveBytes());

            m_myColor = Proto::PieceColorAndType::FromProto(started.game_started().your_color());
            Rebuild(started.game_started().board());
        }

    public:
        static std::unique_ptr<Session> Connect(const std::string& host, unsigned short port)
        {
            return std::unique_ptr<Session>(new Session(host, port));
        }

        ePieceColor GetMyColor() const
        {
            return m_myColor;
        }

        const std::shared_ptr<Chessboard>& GetChessboard() const
        {
            return m_chessboard;
        }

        bool IsMyTurn() const
        {
            return !m_gameOver && m_chessboard->GetSideToMove() == m_myColor;
        }

        bool IsGameOver() const
        {
            return m_gameOver;
        }

        eGameState GetFinalState() const
        {
            return m_finalState;
        }

        bool SubmitMove(const Move& move)
        {
            if (!IsMyTurn() || !ApplyMove(move))
            {
                return false;
            }

            chess::proto::Envelope envelope;
            *envelope.mutable_move() = Proto::Move::ToProto(move);
            m_connection.SendBytes(envelope.SerializeAsString());
            return true;
        }

        eServerEvent ReceiveNext()
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(m_connection.ReceiveBytes());

            switch (envelope.payload_case())
            {
            case chess::proto::Envelope::kMove:
                ApplyMove(Proto::Move::FromProto(envelope.move()));
                return eServerEvent::OpponentMoved;
            case chess::proto::Envelope::kBoardSync:
                Rebuild(envelope.board_sync());
                return eServerEvent::Resynced;
            case chess::proto::Envelope::kBoardCheck:
                if (envelope.board_check().ply() >= m_ply)
                {
                    Rebuild(envelope.board_check());
                }
                return eServerEvent::Resynced;
            case chess::proto::Envelope::kGameOver:
                m_gameOver   = true;
                m_finalState = Proto::Session::FromProto(envelope.game_over().state());
                Rebuild(envelope.game_over().board());
                return eServerEvent::GameOver;
            default:
                return eServerEvent::Resynced;
            }
        }
    };
} // namespace Chess::Client
