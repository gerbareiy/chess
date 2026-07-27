module;
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
export module Chess.E2E.ChessExecutables;
import Chess.Utils.ResourceLocator;

namespace Chess::E2E
{
    export class ChessExecutables
    {
    public:
        static std::filesystem::path GetOwnPath()
        {
#ifdef _WIN32
            wchar_t     buffer[MAX_PATH];
            const DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
            if (length == 0 || length == MAX_PATH)
            {
                throw std::runtime_error("GetModuleFileNameW failed");
            }
            return std::filesystem::path(buffer, buffer + length);
#else
            std::error_code error;
            const auto      ownPath = std::filesystem::read_symlink(SELF_EXECUTABLE_LINK, error);
            if (error)
            {
                throw std::runtime_error(
                    std::string("cannot tell where the test executable lives: reading ") + SELF_EXECUTABLE_LINK + " failed with " + error.message());
            }
            return ownPath;
#endif
        }

        static std::filesystem::path GetOwnDirectory()
        {
            return GetOwnPath().parent_path();
        }

        static std::filesystem::path GetConsolePath()
        {
            return GetExecutablePath("console");
        }

        static std::filesystem::path GetServerPath()
        {
            return GetExecutablePath("server");
        }

        // Looked up exactly the way the server looks it up, so the check here fails whenever the server would.
        static std::optional<std::filesystem::path> TryFindChessboardResource()
        {
            return Utils::ResourceLocator::TryFind(std::filesystem::path("resources") / "chessboard.json", GetOwnPath());
        }

        static void RequireLaunchableBinaries()
        {
            for (const auto& exePath : { GetServerPath(), GetConsolePath() })
            {
                if (!std::filesystem::exists(exePath))
                {
                    throw std::runtime_error("expected to find " + exePath.string() + " next to the e2e test executable, but it is missing");
                }
            }

            if (!TryFindChessboardResource().has_value())
            {
                throw std::runtime_error(
                    "resources/chessboard.json was found neither next to " + GetOwnDirectory().string()
                    + " nor in any of its parent directories, so the server would exit immediately");
            }
        }

    private:
#ifdef _WIN32
        static constexpr std::string_view EXECUTABLE_EXTENSION = ".exe";
#else
        static constexpr std::string_view EXECUTABLE_EXTENSION = "";

        static constexpr const char* SELF_EXECUTABLE_LINK = "/proc/self/exe";
#endif

        static std::filesystem::path GetExecutablePath(const std::string& name)
        {
            return GetOwnDirectory() / (name + std::string(EXECUTABLE_EXTENSION));
        }
    };
} // namespace Chess::E2E
