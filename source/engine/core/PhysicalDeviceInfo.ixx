module;
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.PhysicalDeviceInfo;
import Chess.Engine.QueueFamilyInfo;

namespace Chess::Engine
{
    export struct PhysicalDeviceInfo
    {
        VkPhysicalDevice device = VK_NULL_HANDLE;

        VkPhysicalDeviceProperties       properties = VkPhysicalDeviceProperties();
        VkPhysicalDeviceFeatures         features   = VkPhysicalDeviceFeatures();
        VkPhysicalDeviceMemoryProperties memory     = VkPhysicalDeviceMemoryProperties();

        std::vector<QueueFamilyInfo>       queueFamilies;
        std::vector<VkExtensionProperties> extensions;
    };
} // namespace Chess::Engine
