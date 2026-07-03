module;
#include <array>
#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <boost/asio.hpp>
#include "Envelope.pb.h"
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
        using boost::asio::ip::tcp;

        void SendBytes(tcp::socket& socket, const std::string& payload)
        {
            const auto frame = Framing::Encode(payload);
            boost::asio::write(socket, boost::asio::buffer(frame));
        }

        std::string ReceiveBytes(tcp::socket& socket)
        {
            std::array<uint8_t, Framing::HEADER_SIZE> header{};
            boost::asio::read(socket, boost::asio::buffer(header));

            std::string payload;
            payload.resize(Framing::DecodeLength(header));
            boost::asio::read(socket, boost::asio::buffer(payload.data(), payload.size()));
            return payload;
        }

        chess::proto::Chessboard BoardToProto(const std::shared_ptr<Chess::Chessboard>& board)
        {
            return Chess::Proto::Chessboard::ToProto(board->GetPieceDirector()->GetPiecesOnBoard(), board->GetSideToMove(), 0);
        }

        void ReadFindGame(tcp::socket& socket)
        {
            chess::proto::Envelope envelope;
            envelope.ParseFromString(ReceiveBytes(socket));
        }

        void SendGameStarted(tcp::socket& socket, Chess::ePieceColor color, const std::shared_ptr<Chess::Chessboard>& board)
        {
            chess::proto::Envelope envelope;
            auto*                  started = envelope.mutable_game_started();
            started->set_your_color(Chess::Proto::PieceColorAndType::ToProto(color));
            *started->mutable_board() = BoardToProto(board);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void SendBoardSync(tcp::socket& socket, const std::shared_ptr<Chess::Chessboard>& board)
        {
            chess::proto::Envelope envelope;
            *envelope.mutable_board_sync() = BoardToProto(board);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void SendGameOver(tcp::socket& socket, Chess::eGameState state, const std::shared_ptr<Chess::Chessboard>& board)
        {
            chess::proto::Envelope envelope;
            auto*                  over = envelope.mutable_game_over();
            over->set_state(Chess::Proto::Session::ToProto(state));
            *over->mutable_board() = BoardToProto(board);
            SendBytes(socket, envelope.SerializeAsString());
        }

        void RunMatch(tcp::socket& white, tcp::socket& black, const std::shared_ptr<Chess::Chessboard>& board)
        {
            Chess::GameStateChecker checker;
            tcp::socket*            current  = &white;
            tcp::socket*            opponent = &black;

            try
            {
                while (true)
                {
                    chess::proto::Envelope envelope;
                    envelope.ParseFromString(ReceiveBytes(*current));
                    if (envelope.payload_case() != chess::proto::Envelope::kMove)
                    {
                        continue;
                    }

                    const Chess::Move move = Chess::Proto::Move::FromProto(envelope.move());
                    const bool        valid =
                        board->TrySelectPiece(move.from) && board->TryMovePiece(move.to, std::make_shared<Chess::FixedPromoter>(move.promotion));

                    if (!valid)
                    {
                        // Нелегальный ход (баг или обход клиентской валидации) — откатываем отправителя
                        // к авторитетной доске, ход остаётся за ним.
                        SendBoardSync(*current, board);
                        continue;
                    }

                    chess::proto::Envelope forwarded;
                    *forwarded.mutable_move() = envelope.move();
                    SendBytes(*opponent, forwarded.SerializeAsString());

                    const auto state = checker.Calculate(board);
                    if (state == Chess::eGameState::CHECKMATE || state == Chess::eGameState::DRAW)
                    {
                        SendGameOver(white, state, board);
                        SendGameOver(black, state, board);
                        return;
                    }

                    std::swap(current, opponent);
                }
            }
            catch (const std::exception&)
            {
                // Один из клиентов отключился — завершаем партию.
            }
        }
    } // namespace

    void GameHost::HostSingleMatch(unsigned short port, std::vector<std::shared_ptr<Chess::Piece>> pieces)
    {
        boost::asio::io_context io;
        tcp::acceptor           acceptor(io, tcp::endpoint(tcp::v4(), port));

        tcp::socket white = acceptor.accept();
        tcp::socket black = acceptor.accept();

        ReadFindGame(white);
        ReadFindGame(black);

        const auto board = Chess::ChessboardFactory::Create(std::move(pieces), Chess::ePieceColor::WHITE);

        SendGameStarted(white, Chess::ePieceColor::WHITE, board);
        SendGameStarted(black, Chess::ePieceColor::BLACK, board);

        RunMatch(white, black, board);
    }
} // namespace Chess::Net
