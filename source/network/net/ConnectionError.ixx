module;
#include <stdexcept>
export module Chess.Network.ConnectionError;

namespace Chess::Network
{
    export class ConnectionError : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
} // namespace Chess::Network
