module;
#include <memory>
#include <vector>
export module Chess.Net.GameHost;
import Chess.Piece;

namespace Chess::Net
{
    // Авторитетный хост партии. Реализация (asio + protobuf) вынесена в GameHost.cpp, чтобы типы
    // boost::asio не пересекали границу модулей.
    export class GameHost
    {
    public:
        // Принимает двух клиентов на порту (первый — белые, второй — чёрные), рассылает полную
        // стартовую доску и играет одну партию до терминального состояния либо отключения клиента.
        static void HostSingleMatch(unsigned short port, std::vector<std::shared_ptr<Chess::Piece>> pieces);
    };
} // namespace Chess::Net
