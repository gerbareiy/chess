module;
#include <filesystem>
#include <optional>
export module Chess.Utils.ResourceLocator;

namespace Chess::Utils
{
    export class ResourceLocator
    {
    public:
        // Resources live next to the repository root while binaries sit a few directories deeper, so the file is looked
        // up upwards - first from the executable's own directory, then from the working directory. That way the binary
        // can be started from anywhere, not only from its output directory.
        static std::optional<std::filesystem::path> TryFind(const std::filesystem::path& relativePath, const std::filesystem::path& executablePath)
        {
            const auto nearExecutable = TryFindUpwards(relativePath, GetExecutableDirectory(executablePath));
            if (nearExecutable.has_value())
            {
                return nearExecutable;
            }

            std::error_code error;
            const auto      workingDirectory = std::filesystem::current_path(error);
            if (error)
            {
                return std::nullopt;
            }
            return TryFindUpwards(relativePath, workingDirectory);
        }

    private:
        static std::filesystem::path GetExecutableDirectory(const std::filesystem::path& executablePath)
        {
            if (executablePath.empty())
            {
                return {};
            }

            std::error_code error;
            const auto      absolutePath = std::filesystem::weakly_canonical(executablePath, error);
            return error ? executablePath.parent_path() : absolutePath.parent_path();
        }

        static std::optional<std::filesystem::path> TryFindUpwards(
            const std::filesystem::path& relativePath, const std::filesystem::path& startDirectory)
        {
            auto directory = startDirectory;
            while (!directory.empty())
            {
                std::error_code error;
                const auto      candidate = directory / relativePath;
                if (std::filesystem::exists(candidate, error))
                {
                    return candidate;
                }

                const auto parent = directory.parent_path();
                if (parent == directory)
                {
                    break;
                }
                directory = parent;
            }
            return std::nullopt;
        }
    };
} // namespace Chess::Utils
