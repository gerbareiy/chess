module;
#include <boost/functional/hash.hpp>
#include <cstdint>
#include <functional>
export module Chess.Coordinate;

namespace Chess
{
    export struct Coordinate
    {
        char         file = 'A';
        int rank = 1;

        bool operator==(const Coordinate& other) const = default;
        bool operator!=(const Coordinate& other) const = default;
    };
} // namespace Chess

template <> struct std::hash<Chess::Coordinate>
{
    std::size_t operator()(const Chess::Coordinate& coordinate) const noexcept
    {
        std::size_t result = std::hash<char>{}(coordinate.file);
        boost::hash_combine(result, coordinate.rank);
        return result;
    }
};
