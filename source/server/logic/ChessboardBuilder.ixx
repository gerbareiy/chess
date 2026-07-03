module;
#include <boost/json.hpp>
#include <expected>
#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <ranges>
#include <string>
#include <vector>
export module Chess.ChessboardBuilder;
import Chess.Counts;
import Chess.Coordinate;
import Chess.ePieceColor;
import Chess.ePieceType;
import Chess.King;
import Chess.Piece;
import Chess.PieceColorAndType;
import Chess.PiecePlacement;
import Chess.PiecesOnBoardCreator;
import Chess.PieceTypeConverter;
import Chess.Sizes;
import Chess.Utils.Converter;

namespace Chess
{
    export class ChessboardBuilder
    {
        static boost::json::object GetConfig(const std::string& path)
        {
            std::ifstream file(path);
            if (!file.is_open())
            {
                throw std::invalid_argument("Failed to open configuration file: " + path);
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            boost::json::value value = boost::json::parse(buffer.str());
            return value.as_object();
        }

        static bool IsPieceOnBoard(const std::vector<PiecePlacement>& placements, const Coordinate& coordinate)
        {
            return std::ranges::any_of(placements, [&coordinate](const auto& placement) { return placement.coordinate == coordinate; });
        }

        static void ThrowIfPieceAlreadyOnBoard(const std::vector<PiecePlacement>& placements, const Coordinate& coordinate)
        {
            if (IsPieceOnBoard(placements, coordinate))
            {
                throw std::invalid_argument(std::format("Two pieces cannot occupy the same square: {},{}", coordinate.file, coordinate.rank));
            }
        }

        static Coordinate ParseCoordinate(const boost::json::object& piece, ePieceType type)
        {
            if (!piece.contains("file"))
            {
                throw std::invalid_argument(std::format("{} is missing 'file' in configuration", PieceTypeConverter::ConvertToString(type)));
            }
            if (!piece.contains("rank"))
            {
                throw std::invalid_argument(std::format("{} is missing 'rank' in configuration", PieceTypeConverter::ConvertToString(type)));
            }

            const std::string files = piece.at("file").as_string().c_str();
            if (files.size() != 1)
            {
                throw std::invalid_argument(std::format("{} has invalid 'file' in configuration", PieceTypeConverter::ConvertToString(type)));
            }

            const char        file       = files[0];
            const std::string stringRank = piece.at("rank").as_string().c_str();
            const auto        rank       = Utils::Converter::ToInt32(stringRank);
            if (!rank.has_value())
            {
                throw std::invalid_argument(
                    std::format("{} has invalid 'rank' in configuration, it must be convertible to int", PieceTypeConverter::ConvertToString(type)));
            }
            return Coordinate(file, rank.value());
        }

        static std::shared_ptr<King> ParseKing(const boost::json::object& side, ePieceColor color)
        {
            const auto kings = side.at(PieceTypeConverter::ConvertToConfigString(ePieceType::KING)).as_array();
            if (kings.size() != ONE_COLOR_KINGS_COUNT)
            {
                throw std::invalid_argument("Configuration must contain exactly one king for each color");
            }
            const auto coordinate = ParseCoordinate(kings[0].as_object(), ePieceType::KING);
            return std::make_shared<King>(color, coordinate);
        }

        static void ParsePieces(std::vector<PiecePlacement>& placements, const boost::json::object& side, ePieceColor color, ePieceType type)
        {
            const auto pieceName = PieceTypeConverter::ConvertToConfigString(type);
            if (!side.contains(pieceName))
            {
                return;
            }

            for (const auto& piece : side.at(pieceName).as_array())
            {
                const auto coordinate = ParseCoordinate(piece.as_object(), type);
                ThrowIfPieceAlreadyOnBoard(placements, coordinate);
                placements.push_back({ PieceColorAndType(color, type), coordinate });
            }
        }

        static void ParseSidePieces(std::vector<PiecePlacement>& placements, const boost::json::object& side, ePieceColor color)
        {
            for (const auto& pieceType : PieceTypeConverter::pieceTypes | std::views::filter([](auto type) { return type != ePieceType::KING; }))
            {
                ParsePieces(placements, side, color, pieceType);
            }
        }

    public:
        static std::vector<std::shared_ptr<Piece>> InitBoard(const std::string& configurationPath)
        {
            if (!std::filesystem::exists(configurationPath))
            {
                throw std::invalid_argument("Configuration was not found");
            }

            const auto config = GetConfig(configurationPath);

            const auto whiteSide = config.at("white").as_object();
            const auto whiteKing = ParseKing(whiteSide, ePieceColor::WHITE);

            const auto blackSide = config.at("black").as_object();
            const auto blackKing = ParseKing(blackSide, ePieceColor::BLACK);

            std::vector<PiecePlacement> placements;
            placements.reserve(MAX_ELEMENTS_COUNT);

            ParseSidePieces(placements, whiteSide, ePieceColor::WHITE);
            ParseSidePieces(placements, blackSide, ePieceColor::BLACK);

            return PiecesOnBoardCreator::Create(whiteKing, blackKing, placements);
        }
    };
} // namespace Chess
