module;
#include <format>
#include <magic_enum/magic_enum.hpp>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.VulkanChecker;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class VulkanChecker
    {
    public:
        static void ThrowIfNotSuccess(VkResult result)
        {
            if (result != VK_SUCCESS)
            {
                throw Utils::VulcanException(std::format("{}", magic_enum::enum_name(result)));
            }
        }
    };
} // namespace Chess::Engine
