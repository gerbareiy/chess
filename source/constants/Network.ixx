module;
#include <string_view>
export module Chess.Constants.Network;

namespace Chess::Constants::Network
{
    export constexpr unsigned short DEFAULT_PORT = 5555;

    export constexpr std::string_view DEFAULT_HOST = "127.0.0.1";
} // namespace Chess::Constants::Network
