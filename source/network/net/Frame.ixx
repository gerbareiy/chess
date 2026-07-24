module;
#include <string>
export module Chess.Network.Frame;

namespace Chess::Network
{
    export struct Frame
    {
        std::string identity;
        std::string payload;
        bool        disconnected = false;
    };
} // namespace Chess::Network
