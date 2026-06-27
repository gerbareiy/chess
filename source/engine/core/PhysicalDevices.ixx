module;
#include <memory>
#include <ranges>
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.PhysicalDevices;
import Chess.Engine.PhysicalDeviceInfo;
import Chess.Engine.QueueFamilyInfo;
import Chess.Engine.VulkanChecker;

namespace Chess::Engine
{
    export class PhysicalDevices
    {
        std::vector<PhysicalDeviceInfo> m_devices;

        static std::vector<VkPhysicalDevice> EnumeratePhysicalDevices(const VkInstance& instance)
        {
            uint32_t count = 0u;
            VulkanChecker::ThrowIfNotSuccess(vkEnumeratePhysicalDevices(instance, &count, nullptr));
            auto result = std::vector<VkPhysicalDevice>(count);
            VulkanChecker::ThrowIfNotSuccess(vkEnumeratePhysicalDevices(instance, &count, result.data()));
            return result;
        }

        static std::vector<QueueFamilyInfo> CalculateQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
        {
            uint32_t count = 0u;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
            std::vector<VkQueueFamilyProperties> properties(count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &count, properties.data());

            std::vector<QueueFamilyInfo> result;
            result.reserve(count);

            for (const uint32_t index : std::views::iota(0u, count))
            {
                QueueFamilyInfo info;
                info.index      = index;
                info.queueCount = properties[index].queueCount;
                info.flags      = properties[index].queueFlags;

                VkBool32 presentSupport = VK_FALSE;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, index, surface, &presentSupport);

                info.present = presentSupport == VK_TRUE;
                result.push_back(std::move(info));
            }

            return result;
        }

        static std::vector<VkExtensionProperties> CalculateExtensions(VkPhysicalDevice device)
        {
            uint32_t count = 0;
            VulkanChecker::ThrowIfNotSuccess(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr));
            std::vector<VkExtensionProperties> result(count);
            VulkanChecker::ThrowIfNotSuccess(vkEnumerateDeviceExtensionProperties(device, nullptr, &count, result.data()));
            return result;
        }

        static PhysicalDeviceInfo BuildPhysicalDeviceInfo(VkPhysicalDevice device, VkSurfaceKHR surface)
        {
            PhysicalDeviceInfo info;
            info.device = device;

            vkGetPhysicalDeviceProperties(device, &info.properties);
            vkGetPhysicalDeviceFeatures(device, &info.features);
            vkGetPhysicalDeviceMemoryProperties(device, &info.memory);

            info.queueFamilies = CalculateQueueFamilies(device, surface);
            info.extensions    = CalculateExtensions(device);

            return info;
        }

        PhysicalDevices() = default;

        void Init(const VkInstance& instance, VkSurfaceKHR surface)
        {
            const auto devices = EnumeratePhysicalDevices(instance);
            m_devices.reserve(devices.size());
            for (const auto device : devices)
            {
                m_devices.push_back(BuildPhysicalDeviceInfo(device, surface));
            }
        }

    public:
        static std::unique_ptr<PhysicalDevices> Create(const VkInstance& instance, VkSurfaceKHR surface)
        {
            auto result = std::unique_ptr<PhysicalDevices>(new PhysicalDevices);
            result->Init(instance, surface);
            return result;
        }

        const std::vector<PhysicalDeviceInfo>& GetDevices() const
        {
            return m_devices;
        }
    };
} // namespace Chess::Engine
