module;
#include <string>
export module Chess.Net.Frame;

namespace Chess::Net
{
    export struct Frame
    {
        std::string identity;
        std::string payload;
        bool        disconnected = false;
    };
} // namespace Chess::Net
