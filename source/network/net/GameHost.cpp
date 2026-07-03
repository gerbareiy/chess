module;
#include "Envelope.pb.h"
#include <array>
#include <boost/asio.hpp>
#include <boost/system/system_error.hpp>
#include <memory>
#include <string>
#include <utility>
#include <vector>
module Chess.Net.GameHost;
import Chess.Chessboard;
import Chess.ChessboardFactory;
import Chess.eGameState;
import Chess.ePieceColor;
import Chess.FixedPromoter;
import Chess.GameStateChecker;
import Chess.Move;
import Chess.Net.Framing;
import Chess.PieceDirector;
import Chess.Proto.Chessboard;
import Chess.Proto.Move;
import Chess.Proto.PieceColorAndType;
import Chess.Proto.Session;

namespace Chess::Net
{
    namespace
    {
        void SendBytes(boost::asio::ip::tcp::socket& socket, const std::string& payload)
        {
            const auto frame = Framing::Encode(payload);
            boost::asio::write(socket, boost::asio::buffer(frame));
        }

        std::string ReceiveBytes(boost::asio::ip::tcp::socket& socket)
        {
            std::array<uint8_t, Framing::HEADER_SIZE> header{};
            boost::asio::read(socket, boost::asio::buffer(header));

            std::string payload;
            payload.resize(Framing::DecodeLength(header));
            boost::asio::read(socket, boost::asio::buffer(payload.data(), payload.size()));
            return payload;
        }

        constexpr uint32_t BOARD_CHECK_PERIOD = 10;

        chess::proto::Chessboard BoardToProto(const std::shared_ptr<Chessboard>& board, uint32_t ply)
        {
            return Proto::Chessboard::ToProto(board->GetPieceDirector()->GetPiecesOnBoard(), board->GetSideToMove(), 0, ply);
        }

        void ReadFindGame(boost::asio::ip::tcp::socket& socket)
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(ReceiveBytes(socket));
        }

        void SendGameStarted(boost::asio::ip::tcp::socket& socket, ePieceColor color, const std::shared_ptr<Chessboard>& board, uint32_t ply)
        {
            auto  envelope = chess::proto::Envelope();
            auto* started  = envelope.mutable_game_started();
            started->set_your_color(Proto::PieceColorAndType::ToProto(color));
            *started->mutable_board() = BoardToProto(board, ply);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void SendBoardSync(boost::asio::ip::tcp::socket& socket, const std::shared_ptr<Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_sync() = BoardToProto(board, ply);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void SendBoardCheck(boost::asio::ip::tcp::socket& socket, const std::shared_ptr<Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_check() = BoardToProto(board, ply);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void SendGameOver(boost::asio::ip::tcp::socket& socket, eGameState state, const std::shared_ptr<Chessboard>& board, uint32_t ply)
        {
            chess::proto::Envelope envelope;
            auto*                  over = envelope.mutable_game_over();
            over->set_state(Proto::Session::ToProto(state));
            *over->mutable_board() = BoardToProto(board, ply);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void RunMatch(boost::asio::ip::tcp::socket& white, boost::asio::ip::tcp::socket& black, const std::shared_ptr<Chess::Chessboard>& board)
        {
            GameStateChecker              checker;
            boost::asio::ip::tcp::socket* current  = &white;
            boost::asio::ip::tcp::socket* opponent = &black;
            uint32_t                      ply      = 0;

            while (true)
            {
                chess::proto::Envelope incoming;
                try
                {
                    incoming.ParseFromString(ReceiveBytes(*current));
                }
                catch (const boost::system::system_error&)
                {
                    // current player disconnected, end the match.
                    return;
                }

                if (incoming.payload_case() != chess::proto::Envelope::kMove)
                {
                    continue;
                }

                const auto [from, to, promotion] = Proto::Move::FromProto(incoming.move());
                const bool valid                 = board->TrySelectPiece(from) && board->TryMovePiece(to, std::make_shared<FixedPromoter>(promotion));

                auto state = eGameState::PLAYING;
                if (valid)
                {
                    ++ply;
                    state = checker.Calculate(board);
                }

                try
                {
                    if (!valid)
                    {
                        SendBoardSync(*current, board, ply);
                        continue;
                    }

                    chess::proto::Envelope forwarded;
                    *forwarded.mutable_move() = incoming.move();
                    SendBytes(*opponent, forwarded.SerializeAsString());

                    if (state == eGameState::CHECKMATE || state == Chess::eGameState::DRAW)
                    {
                        SendGameOver(white, state, board, ply);
                        SendGameOver(black, state, board, ply);
                        return;
                    }

                    if (ply % BOARD_CHECK_PERIOD == 0)
                    {
                        SendBoardCheck(white, board, ply);
                        SendBoardCheck(black, board, ply);
                    }
                }
                catch (const boost::system::system_error&)
                {
                    // one of the players disconnected, end the match.
                    return;
                }

                std::swap(current, opponent);
            }
        }
    } // namespace

    void GameHost::HostSingleMatch(unsigned short port, std::vector<std::shared_ptr<Piece>> pieces)
    {
        auto io       = boost::asio::io_context();
        auto acceptor = boost::asio::ip::tcp::acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

        boost::asio::ip::tcp::socket white = acceptor.accept();
        boost::asio::ip::tcp::socket black = acceptor.accept();

        ReadFindGame(white);
        ReadFindGame(black);

        const auto board = ChessboardFactory::Create(std::move(pieces), ePieceColor::WHITE);

        SendGameStarted(white, ePieceColor::WHITE, board, 0);
        SendGameStarted(black, ePieceColor::BLACK, board, 0);

        RunMatch(white, black, board);
    }
} // namespace Chess::Net
