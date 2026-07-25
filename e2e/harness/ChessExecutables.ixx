module;
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <filesystem>
#include <stdexcept>
export module Chess.E2E.ChessExecutables;

namespace Chess::E2E
{
    export class ChessExecutables
    {
    public:
        static std::filesystem::path GetOwnDirectory()
        {
            wchar_t     buffer[MAX_PATH];
            const DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
            if (length == 0 || length == MAX_PATH)
            {
                throw std::runtime_error("GetModuleFileNameW failed");
            }
            return std::filesystem::path(buffer, buffer + length).parent_path();
        }

        static std::filesystem::path GetConsolePath()
        {
            return GetOwnDirectory() / "console.exe";
        }

        static std::filesystem::path GetServerPath()
        {
            return GetOwnDirectory() / "server.exe";
        }

        static std::filesystem::path GetChessboardResourcePath()
        {
            return GetOwnDirectory().parent_path().parent_path().parent_path() / "resources" / "chessboard.json";
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

            const auto resourcePath = GetChessboardResourcePath();
            if (!std::filesystem::exists(resourcePath))
            {
                throw std::runtime_error(
                    "server.exe resolves its starting position as <working directory>/../../../resources/chessboard.json, which for working "
                    "directory "
                    + GetOwnDirectory().string() + " means " + resourcePath.string()
                    + " - and that file does not exist, so the "
                      "server would exit immediately");
            }
        }
    };
} // namespace Chess::E2E
