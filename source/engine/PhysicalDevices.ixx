module;
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.PhysicalDevices;
import Chess.Engine.VulkanChecker;

namespace Chess::Engine
{
    export class PhysicalDevices
    {
        std::vector<VkPhysicalDevice>           m_devices;
        std::vector<VkPhysicalDeviceProperties> m_devicesProperties;
        std::vector<VkPhysicalDeviceFeatures>   m_devicesFeatures;

        static std::vector<VkPhysicalDevice> CalculatePhysicalDevices(const VkInstance& instance)
        {
            uint32_t count = 0;
            VulkanChecker::ThrowIfNotSuccess(vkEnumeratePhysicalDevices(instance, std::addressof(count), nullptr));
            std::vector<VkPhysicalDevice> devices(count);
            VulkanChecker::ThrowIfNotSuccess(vkEnumeratePhysicalDevices(instance, std::addressof(count), devices.data()));
            return devices;
        }

        static std::vector<VkPhysicalDeviceProperties> CalculatePhysicalDevicesProperties(const std::vector<VkPhysicalDevice>& devices)
        {
            auto result = std::vector<VkPhysicalDeviceProperties>();
            result.reserve(devices.size());
            for (const auto& device : devices)
            {
                auto properties = VkPhysicalDeviceProperties();
                vkGetPhysicalDeviceProperties(device, std::addressof(properties));
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
                vkGetPhysicalDeviceFeatures(device, std::addressof(features));
                result.push_back(std::move(features));
            }
            return result;
        }

        PhysicalDevices() = default;

        void Init(const VkInstance& instance)
        {
            m_devices           = CalculatePhysicalDevices(instance);
            m_devicesProperties = CalculatePhysicalDevicesProperties(m_devices);
            m_devicesFeatures   = CalculatePhysicalDevicesFeatures(m_devices);
        }

    public:
        static std::unique_ptr<PhysicalDevices> Create(const VkInstance& instance)
        {
            auto result = std::unique_ptr<PhysicalDevices>(new PhysicalDevices);
            result->Init(instance);
            return result;
        }

        const std::vector<VkPhysicalDevice>& GetPhysicalDevices() const
        {
            return m_devices;
        }

        const std::vector<VkPhysicalDeviceFeatures>& GetPhysicalDevicesFeatures() const
        {
            return m_devicesFeatures;
        }
    };
} // namespace Chess::Engine
