module;
#include <memory>
#include <vector>
export module Chess.Net.GameHost;
import Chess.Piece;

namespace Chess::Net
{
    export class GameHost
    {
    public:
        static void HostSingleMatch(unsigned short port, std::vector<std::shared_ptr<Chess::Piece>> pieces);
    };
} // namespace Chess::Net
