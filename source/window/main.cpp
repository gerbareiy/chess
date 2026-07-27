import Chess.Engine.ApplicationInformation;
import Chess.Engine.Engine;

int main()
{
    auto engine = Chess::Engine::Engine::Create(
        Chess::Engine::ApplicationInformation::APP_NAME.data(),
        Chess::Engine::ApplicationInformation::APP_VERSION,
        Chess::Engine::ApplicationInformation::ENGINE_NAME.data(),
        Chess::Engine::ApplicationInformation::ENGINE_VERSION,
        Chess::Engine::ApplicationInformation::API_VERSION);
    while (engine.NeedUpdate())
    {
        engine.Update();
    }

    return 0;
}
