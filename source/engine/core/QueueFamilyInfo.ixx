module;
#include <vulkan/vulkan_core.h>
export module Chess.Engine.QueueFamilyInfo;

namespace Chess::Engine
{
    export struct QueueFamilyInfo
    {
        uint32_t index      = 0u;
        uint32_t queueCount = 0u;

        VkQueueFlags flags = 0u;

        bool present = false;
    };
} // namespace Chess::Engine
