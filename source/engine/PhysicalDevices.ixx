module;
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.PhysicalDevices;
import Chess.Engine.VulkanChecker;

namespace Chess::Engine
{
    export class PhysicalDevices
    {
        std::vector<VkPhysicalDevice>           m_physicalDevices;
        std::vector<VkPhysicalDeviceProperties> m_physicalDevicesProperties;
        std::vector<VkPhysicalDeviceFeatures>   m_physicalDevicesFeatures;

        static std::vector<VkPhysicalDevice> CalculatePhysicalDevices(const VkInstance& instance)
        {
            uint32_t count = 0;
            VulkanChecker::ThrowIfNotSuccess(vkEnumeratePhysicalDevices(instance, &count, nullptr));
            std::vector<VkPhysicalDevice> devices(count);
            VulkanChecker::ThrowIfNotSuccess(vkEnumeratePhysicalDevices(instance, &count, devices.data()));
            return devices;
        }

        static std::vector<VkPhysicalDeviceProperties> CalculatePhysicalDevicesProperties(const std::vector<VkPhysicalDevice>& devices)
        {
            auto result = std::vector<VkPhysicalDeviceProperties>();
            result.reserve(devices.size());
            for (const auto& device : devices)
            {
                auto properties = VkPhysicalDeviceProperties();
                vkGetPhysicalDeviceProperties(device, &properties);
                result.push_back(std::move(properties));
            }
            return result;
        }

        static std::vector<VkPhysicalDeviceFeatures> CalculatePhysicalDevicesFeatures(const std::vector<VkPhysicalDevice>& devices)
        {
            auto result = std::vector<VkPhysicalDeviceFeatures>();
            result.reserve(devices.size());
            for (const auto& device : devices)
            {
                auto features = VkPhysicalDeviceFeatures();
                vkGetPhysicalDeviceFeatures(device, &features);
                result.push_back(std::move(features));
            }
            return result;
        }

        void Init(const VkInstance& instance)
        {
            m_physicalDevices           = CalculatePhysicalDevices(instance);
            m_physicalDevicesProperties = CalculatePhysicalDevicesProperties(m_physicalDevices);
            m_physicalDevicesFeatures   = CalculatePhysicalDevicesFeatures(m_physicalDevices);
        }

    public:
        static PhysicalDevices Create(const VkInstance& instance)
        {
            auto result = PhysicalDevices();
            result.Init(instance);
            return result;
        }

        const std::vector<VkPhysicalDevice>& GetPhysicalDevices() const
        {
            return m_physicalDevices;
        }

        const std::vector<VkPhysicalDeviceFeatures>& GetPhysicalDevicesFeatures() const
        {
            return m_physicalDevicesFeatures;
        }
    };
} // namespace Chess::Engine
