import Chess.Constants.ApplicationInformation;
import Chess.Engine.Engine;

int main()
{
    auto engine = Chess::Engine::Engine::Create(
        Chess::Constants::ApplicationInformation::APP_NAME.data(),
        Chess::Constants::ApplicationInformation::APP_VERSION,
        Chess::Constants::ApplicationInformation::ENGINE_NAME.data(),
        Chess::Constants::ApplicationInformation::ENGINE_VERSION,
        Chess::Constants::ApplicationInformation::API_VERSION);
    while (engine.NeedUpdate())
    {
        engine.Update();
    }

    return 0;
}
