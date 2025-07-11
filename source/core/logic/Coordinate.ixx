module;
#include <functional>
export module Chess.Coordinate;

namespace Chess
{
    export struct Coordinate
    {
        char file = 'A';
        int  rank = 1;

        bool operator==(const Coordinate& other) const
        {
            return file == other.file && rank == other.rank;
        }

        bool operator!=(const Coordinate& other) const
        {
            return !(*this == other);
        }
    };
} // namespace Chess

template <> struct std::hash<Chess::Coordinate>
{
    size_t operator()(const Chess::Coordinate& coordinate) const noexcept
    {
        const size_t hashOne = std::hash<char>{}(coordinate.file);
        const size_t hashTwo = std::hash<int>{}(coordinate.rank);
        return hashOne ^ (hashTwo << 1);
    }
};
