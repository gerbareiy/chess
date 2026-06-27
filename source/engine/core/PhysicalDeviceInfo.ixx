module;
#include <string>
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

        std::vector<QueueFamilyInfo> queueFamilies;

        std::vector<VkExtensionProperties> extensions;

        uint32_t vendorID = 0;
        uint32_t deviceID = 0;

        std::string deviceName;
    };
} // namespace Chess::Engine
