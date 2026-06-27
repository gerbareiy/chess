module;
#include <memory>
#include <ranges>
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.LogicalDevices;
import Chess.Engine.VulkanChecker;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class LogicalDevices
    {
        static constexpr float                            m_priority = 1.f;
        std::vector<std::vector<VkDeviceQueueCreateInfo>> m_queueCreateInfos;
        std::vector<VkDeviceCreateInfo>                   m_createInfos;
        std::vector<VkDevice>                             m_devices;

        static std::vector<VkDeviceQueueCreateInfo> CalculateQueueCreateInfos(const VkPhysicalDevice& device)
        {
            uint32_t count = 0u;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
            auto queueFamilies = std::vector<VkQueueFamilyProperties>(count);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());

            auto result = std::vector<VkDeviceQueueCreateInfo>();
            result.reserve(queueFamilies.size());

            for (const uint32_t index : std::views::iota(0u, queueFamilies.size()))
            {
                auto queueInfo             = VkDeviceQueueCreateInfo();
                queueInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex = index;
                queueInfo.queueCount       = 1;
                queueInfo.pQueuePriorities = &m_priority;
                result.push_back(std::move(queueInfo));
            }
            return result;
        }

        static std::vector<std::vector<VkDeviceQueueCreateInfo>> CalculateAllQueueCreateInfos(const std::vector<VkPhysicalDevice>& devices)
        {
            auto result = std::vector<std::vector<VkDeviceQueueCreateInfo>>();
            result.reserve(devices.size());
            for (const auto& device : devices)
            {
                result.push_back(CalculateQueueCreateInfos(device));
            }
            return result;
        }

        static std::vector<VkDeviceCreateInfo> CalculateDeviceCreateInfos(
            const std::vector<VkPhysicalDeviceFeatures>& features, const std::vector<std::vector<VkDeviceQueueCreateInfo>>& queueInfos)
        {
            if (features.size() != queueInfos.size())
            {
                throw Utils::NotEqualSizeException();
            }

            auto result = std::vector<VkDeviceCreateInfo>();
            result.reserve(features.size());

            for (const size_t index : std::views::iota(0u, features.size()))
            {
                auto info                 = VkDeviceCreateInfo();
                info.sType                = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                info.queueCreateInfoCount = static_cast<uint32_t>(queueInfos[index].size());
                info.pQueueCreateInfos    = queueInfos[index].data();

                // We don't need extensions for a while.
                info.enabledExtensionCount   = 0;
                info.ppEnabledExtensionNames = nullptr;

                info.pEnabledFeatures = &features[index];
                result.push_back(std::move(info));
            }
            return result;
        }

        static std::vector<VkDevice> CalculateLogicalDevices(
            const std::vector<VkPhysicalDevice>& devices, const std::vector<VkDeviceCreateInfo>& infos)
        {
            if (devices.size() != infos.size())
            {
                throw Utils::NotEqualSizeException();
            }

            auto result = std::vector<VkDevice>();
            result.reserve(devices.size());

            for (const size_t index : std::views::iota(0u, devices.size()))
            {
                VkDevice logical;
                VulkanChecker::ThrowIfNotSuccess(vkCreateDevice(devices[index], &infos[index], nullptr, &logical));
                result.push_back(std::move(logical));
            }
            return result;
        }

        LogicalDevices() = default;

        void Init(const std::vector<VkPhysicalDevice>& physicalDevices, const std::vector<VkPhysicalDeviceFeatures>& physicalDevicesFeatures)
        {
            m_queueCreateInfos = CalculateAllQueueCreateInfos(physicalDevices);
            m_createInfos      = CalculateDeviceCreateInfos(physicalDevicesFeatures, m_queueCreateInfos);
            m_devices          = CalculateLogicalDevices(physicalDevices, m_createInfos);
        }

    public:
        static std::unique_ptr<LogicalDevices> Create(
            const std::vector<VkPhysicalDevice>& physicalDevices, const std::vector<VkPhysicalDeviceFeatures>& physicalDevicesFeatures)
        {
            auto result = std::unique_ptr<LogicalDevices>(new LogicalDevices);
            result->Init(physicalDevices, physicalDevicesFeatures);
            return result;
        }
    };
} // namespace Chess::Engine
