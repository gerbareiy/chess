module;
#include <boost/functional/hash.hpp>
#include <cstdint>
#include <functional>
export module Chess.Core.Coordinate;

namespace Chess::Core
{
    export struct Coordinate
    {
        char file = 'A';
        int  rank = 1;

        bool operator==(const Coordinate& other) const = default;
        bool operator!=(const Coordinate& other) const = default;
    };
} // namespace Chess::Core

template <> struct std::hash<Chess::Core::Coordinate>
{
    std::size_t operator()(const Chess::Core::Coordinate& coordinate) const noexcept
    {
        std::size_t result = std::hash<char>{}(coordinate.file);
        boost::hash_combine(result, coordinate.rank);
        return result;
    }
};
