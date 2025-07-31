module;
#include <boost/json.hpp>

#include <expected>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
export module Chess.ChessboardBuilder;
import Chess.Bishop;
import Chess.Counts;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.King;
import Chess.Knight;
import Chess.Pawn;
import Chess.Piece;
import Chess.Queen;
import Chess.Rook;
import Chess.Sizes;

namespace Chess
{
    export class ChessboardBuilder
    {
    private:
        static std::expected<boost::json::object, std::string> GetConfig(std::string path)
        {
            std::ifstream file(path);

            if (!file.is_open())
            {
                return std::unexpected("Failed to open configuration file");
            }
            std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

            boost::json::object json;
            try
            {
                auto tempValue = boost::json::parse(file_contents);
                json           = tempValue.as_object();
            }
            catch (const std::exception&)
            {
                return std::unexpected("Failed to parse configuration");
            }
            return json;
        }

    public:
        static std::vector<std::shared_ptr<Piece>> InitNormalBoard()
        {
            std::vector<std::shared_ptr<Piece>> piecesOnBoard;
            std::shared_ptr<King>               whiteKing;
            std::shared_ptr<King>               blackKing;

            piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);

            std::filesystem::path resourcePath = std::filesystem::current_path().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";
            std::string           path         = resourcePath.string();
            if (!std::filesystem::exists(path))
            {
                std::cerr << "Configuration was not found\n";
                return {};
            }

            boost::json::object config;
            auto                expectedConfig = GetConfig(path);
            if (expectedConfig.has_value())
            {
                config = expectedConfig.value();
            }
            else
            {
                std::cerr << expectedConfig.error();
                return {};
            }

            auto addPieces = [&](boost::json::object side, std::string pieceName, ePieceColor color, ePieceType pieceType) mutable
            {
                auto addPiece = [&](ePieceColor color, ePieceType pieceType, Coordinate coord) mutable
                {
                    switch (pieceType)
                    {
                    case Chess::ePieceType::ROOK:
                        if (color == ePieceColor::WHITE)
                        {
                            if (whiteKing)
                            {
                                piecesOnBoard.push_back(std::make_shared<Rook>(color, coord, whiteKing));
                            }
                        }
                        else if (color == ePieceColor::BLACK)
                        {
                            if (blackKing)
                            {
                                piecesOnBoard.push_back(std::make_shared<Rook>(color, coord, blackKing));
                            }
                        }
                        break;
                    case Chess::ePieceType::QUEEN:
                        piecesOnBoard.push_back(std::make_shared<Queen>(color, coord));
                        break;
                    case Chess::ePieceType::PAWN:
                        piecesOnBoard.push_back(std::make_shared<Pawn>(color, coord));
                        break;
                    case Chess::ePieceType::KNIGHT:
                        piecesOnBoard.push_back(std::make_shared<Knight>(color, coord));
                        break;
                    case Chess::ePieceType::KING:
                        if (color == ePieceColor::WHITE)
                        {
                            whiteKing = std::make_shared<King>(color, coord);
                            piecesOnBoard.push_back(whiteKing);
                        }
                        else if (color == ePieceColor::BLACK)
                        {
                            blackKing = std::make_shared<King>(color, coord);
                            piecesOnBoard.push_back(blackKing);
                        }
                        break;
                    case Chess::ePieceType::BISHOP:
                        piecesOnBoard.push_back(std::make_shared<Bishop>(color, coord));
                        break;
                    case Chess::ePieceType::NONE:
                        break;
                    }
                };
                if (side.contains(pieceName))
                {
                    auto Pieces = side[pieceName].as_array();
                    for (const auto& piece : Pieces)
                    {
                        auto pieceAsObj = piece.as_object();
                        auto file       = *pieceAsObj["file"].as_string().c_str();
                        auto rank       = atoi(pieceAsObj["rank"].as_string().c_str());
                        addPiece(color, pieceType, Coordinate(file, rank));
                    }
                }
            };

            auto whiteSide = config["white"].as_object();

            addPieces(whiteSide, "kings", ePieceColor::WHITE, ePieceType::KING);
            addPieces(whiteSide, "pawns", ePieceColor::WHITE, ePieceType::PAWN);
            addPieces(whiteSide, "rooks", ePieceColor::WHITE, ePieceType::ROOK);
            addPieces(whiteSide, "queens", ePieceColor::WHITE, ePieceType::QUEEN);
            addPieces(whiteSide, "knights", ePieceColor::WHITE, ePieceType::KNIGHT);
            addPieces(whiteSide, "bishops", ePieceColor::WHITE, ePieceType::BISHOP);

            auto blackSide = config["black"].as_object();

            addPieces(blackSide, "kings", ePieceColor::BLACK, ePieceType::KING);
            addPieces(blackSide, "pawns", ePieceColor::BLACK, ePieceType::PAWN);
            addPieces(blackSide, "rooks", ePieceColor::BLACK, ePieceType::ROOK);
            addPieces(blackSide, "queens", ePieceColor::BLACK, ePieceType::QUEEN);
            addPieces(blackSide, "knights", ePieceColor::BLACK, ePieceType::KNIGHT);
            addPieces(blackSide, "bishops", ePieceColor::BLACK, ePieceType::BISHOP);

            return piecesOnBoard;
        }
    };
} // namespace Chess
