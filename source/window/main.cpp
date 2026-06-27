import Chess.Engine.Engine;
import Chess.Window.AppInfo;

int main()
{
    auto engine = Chess::Engine::Engine::Create(
        Chess::Window::AppInfo::APP_NAME.data(),
        Chess::Window::AppInfo::APP_VERSION,
        Chess::Window::AppInfo::ENGINE_NAME.data(),
        Chess::Window::AppInfo::ENGINE_VERSION,
        Chess::Window::AppInfo::API_VERSION);

    while (engine.NeedUpdate())
    {
        engine.Update();
    }

    return 0;
}
