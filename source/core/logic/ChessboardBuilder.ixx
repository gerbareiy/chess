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
import Chess.PieceTypeConverter;
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
        static std::expected<boost::json::object, std::string> GetConfig(const std::string& path)
        {
            std::ifstream file(path);

            if (!file.is_open())
            {
                return std::unexpected("Failed to open configuration file");
            }
            const std::string fileContents((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());

            try
            {
                auto tempValue = boost::json::parse(fileContents);
                return tempValue.as_object();
            }
            catch (const std::exception&)
            {
                return std::unexpected("Failed to parse configuration");
            }
        }

        static bool IsPieceOnBoard(const std::vector<std::shared_ptr<Piece>>& piecesOnBoard, const Coordinate& coordinate)
        {
            return std::ranges::any_of(piecesOnBoard, [&](const auto& piece) { return piece->GetPosition() == coordinate; });
        }

        static void AddPiece(std::vector<std::shared_ptr<Piece>>& piecesOnBoard,
                             ePieceColor                          color,
                             ePieceType                           pieceType,
                             const Coordinate&                    coordinate,
                             std::shared_ptr<King>&               king)
        {
            if (IsPieceOnBoard(piecesOnBoard, coordinate))
            {
                std::cerr << "Cannot upload the piece " << PieceTypeConverter::ConvertToNormalString(pieceType) << ", the coordinate " << coordinate.file
                          << " : " << coordinate.rank << " already exist"
                          << "\n ";
                return;
            }
            switch (pieceType)
            {
            case ePieceType::ROOK:
                piecesOnBoard.push_back(std::make_shared<Rook>(king->GetColorAndType().color, coordinate, king));
                break;
            case ePieceType::QUEEN:
                piecesOnBoard.push_back(std::make_shared<Queen>(color, coordinate));
                break;
            case ePieceType::PAWN:
                piecesOnBoard.push_back(std::make_shared<Pawn>(color, coordinate));
                break;
            case ePieceType::KNIGHT:
                piecesOnBoard.push_back(std::make_shared<Knight>(color, coordinate));
                break;
            case ePieceType::KING:
                king = std::make_shared<King>(color, coordinate);
                piecesOnBoard.push_back(king);
                break;
            case ePieceType::BISHOP:
                piecesOnBoard.push_back(std::make_shared<Bishop>(color, coordinate));
                break;
            case ePieceType::NONE:
                break;
            }
        }

        static void AddPieces(std::vector<std::shared_ptr<Piece>>& piecesOnBoard,
                              const boost::json::object&           side,
                              const std::string&                   pieceName,
                              ePieceColor                          color,
                              ePieceType                           pieceType,
                              std::shared_ptr<King>&               king)
        {
            if (side.contains(pieceName))
            {
                auto pieces = side.at(pieceName).as_array();
                for (const auto& piece : pieces)
                {
                    auto pieceAsObj = piece.as_object();
                    if (!pieceAsObj.contains("file") || !pieceAsObj.contains("rank"))
                    {
                        std::cerr << "Error: " << PieceTypeConverter::ConvertToNormalString(pieceType) << " is missing 'file' or 'rank' in configuration\n";
                        continue;
                    }
                    const auto file = *pieceAsObj["file"].as_string().c_str();
                    const auto rank = atoi(pieceAsObj["rank"].as_string().c_str());
                    AddPiece(piecesOnBoard, color, pieceType, Coordinate(file, rank), king);
                }
            }
        }

    public:
        static std::vector<std::shared_ptr<Piece>> InitBoard(const std::string& configurationPath)
        {
            std::vector<std::shared_ptr<Piece>> piecesOnBoard;
            std::shared_ptr<King>               whiteKing;
            std::shared_ptr<King>               blackKing;

            piecesOnBoard.reserve(MAX_COUNT_ELEMENTS);

            if (!std::filesystem::exists(configurationPath))
            {
                std::cerr << "Configuration was not found\n";
                return {};
            }

            auto expectedConfig = GetConfig(configurationPath);
            if (!expectedConfig.has_value())
            {
                std::cerr << expectedConfig.error();
                return {};
            }
            boost::json::object config = expectedConfig.value();

            const auto whiteSide = config["white"].as_object();
            const auto blackSide = config["black"].as_object();

            for (const auto& pieceType : PieceTypeConverter::pieceTypes)
            {
                AddPieces(piecesOnBoard, whiteSide, PieceTypeConverter::ConvertToConfigString(pieceType), ePieceColor::WHITE, pieceType, whiteKing);
                AddPieces(piecesOnBoard, blackSide, PieceTypeConverter::ConvertToConfigString(pieceType), ePieceColor::BLACK, pieceType, blackKing);
            }

            return piecesOnBoard;
        }
    };
} // namespace Chess
