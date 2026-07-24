module;
#include <stdexcept>
export module Chess.Net.ConnectionError;

namespace Chess::Net
{
    export class ConnectionError : public std::runtime_error
    {
    public:
        using std::runtime_error::runtime_error;
    };
} // namespace Chess::Net
