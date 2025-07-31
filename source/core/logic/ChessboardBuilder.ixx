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

        static bool IsPieceOnBoard(const std::vector<std::shared_ptr<Piece>> piecesOnBoard, Coordinate coord)
        {
            for (const auto& piece : piecesOnBoard)
            {
                if (piece->GetPosition() == coord)
                {
                    return true;
                }
            }
            return false;
        }

        static void AddPiece(std::vector<std::shared_ptr<Piece>>& piecesOnBoard,
                             ePieceColor                          color,
                             ePieceType                           pieceType,
                             Coordinate                           coord,
                             std::shared_ptr<King>&               king)
        {
            if (IsPieceOnBoard(piecesOnBoard, coord))
            {
                std::cerr << "Cannot upload the piece, the coordinate " << coord.file << ": " << coord.rank << " already excists" << "\n";
                return;
            }
            switch (pieceType)
            {
            case Chess::ePieceType::ROOK:
                piecesOnBoard.push_back(std::make_shared<Rook>(king.get()->GetColorAndType().color, coord, king));
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
                king = std::make_shared<King>(color, coord);
                piecesOnBoard.push_back(king);
                break;
            case Chess::ePieceType::BISHOP:
                piecesOnBoard.push_back(std::make_shared<Bishop>(color, coord));
                break;
            case Chess::ePieceType::NONE:
                break;
            }
        }

        static void AddPieces(std::vector<std::shared_ptr<Piece>>& piecesOnBoard,
                              boost::json::object                  side,
                              std::string                          pieceName,
                              ePieceColor                          color,
                              ePieceType                           pieceType,
                              std::shared_ptr<King>&               king)
        {
            if (side.contains(pieceName))
            {
                auto Pieces = side[pieceName].as_array();
                for (const auto& piece : Pieces)
                {
                    auto pieceAsObj = piece.as_object();
                    auto file       = *pieceAsObj["file"].as_string().c_str();
                    auto rank       = atoi(pieceAsObj["rank"].as_string().c_str());
                    AddPiece(piecesOnBoard, color, pieceType, Coordinate(file, rank), king);
                }
            }
        }

        static std::string TypeToString(ePieceType pieceType)
        {
            switch (pieceType)
            {
            case Chess::ePieceType::ROOK:
                return "Rook";
                break;
            case Chess::ePieceType::QUEEN:
                return "Queen";
                break;
            case Chess::ePieceType::PAWN:
                return "Pawn";
                break;
            case Chess::ePieceType::KNIGHT:
                return "Knight";
                break;
            case Chess::ePieceType::KING:
                return "King";
                break;
            case Chess::ePieceType::BISHOP:
                return "Bishop";
                break;
            case Chess::ePieceType::NONE:
                return "None";
                break;
            default:
                return "Unknown";
                break;
            }
        }

        static std::string TypeToStringConfig(ePieceType pieceType)
        {
            switch (pieceType)
            {
            case Chess::ePieceType::ROOK:
                return "rooks";
                break;
            case Chess::ePieceType::QUEEN:
                return "queens";
                break;
            case Chess::ePieceType::PAWN:
                return "pawns";
                break;
            case Chess::ePieceType::KNIGHT:
                return "knights";
                break;
            case Chess::ePieceType::KING:
                return "kings";
                break;
            case Chess::ePieceType::BISHOP:
                return "bishops";
                break;
            case Chess::ePieceType::NONE:
                return "none";
                break;
            default:
                return "unknown";
                break;
            }
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

            std::array<ePieceType, 6> tPieces   = { ePieceType::KING,  ePieceType::PAWN,   ePieceType::ROOK,
                                                    ePieceType::QUEEN, ePieceType::KNIGHT, ePieceType::BISHOP };

            auto whiteSide = config["white"].as_object();
            auto blackSide = config["black"].as_object();


            for (const auto& pieceType : tPieces)
            {
                AddPieces(piecesOnBoard, whiteSide, TypeToStringConfig(pieceType), ePieceColor::WHITE, pieceType, whiteKing);
                AddPieces(piecesOnBoard, blackSide, TypeToStringConfig(pieceType), ePieceColor::BLACK, pieceType, blackKing);
            }

            return piecesOnBoard;
        }
    };
} // namespace Chess
