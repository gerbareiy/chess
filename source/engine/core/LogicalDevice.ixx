module;
#include <memory>
#include <ranges>
#include <set>
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.LogicalDevice;
import Chess.Engine.PhysicalDeviceInfo;
import Chess.Engine.VulkanChecker;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class LogicalDevice
    {
        static constexpr float m_priority = 1.f;

        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        VkQueue m_presentQueue  = VK_NULL_HANDLE;
        VkQueue m_computeQueue  = VK_NULL_HANDLE;
        VkQueue m_transferQueue = VK_NULL_HANDLE;

        static std::vector<VkDeviceQueueCreateInfo> CalculateQueueCreateInfos(const PhysicalDeviceInfo& device)
        {
            std::set<uint32_t> uniqueFamilies;

            for (const auto& family : device.queueFamilies)
            {
                if (family.flags & (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT | VK_QUEUE_TRANSFER_BIT) || family.present)
                {
                    uniqueFamilies.insert(family.index);
                }
            }

            std::vector<VkDeviceQueueCreateInfo> result;
            result.reserve(uniqueFamilies.size());

            for (const uint32_t family : uniqueFamilies)
            {
                VkDeviceQueueCreateInfo info{};
                info.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                info.queueFamilyIndex = family;
                info.queueCount       = 1u;
                info.pQueuePriorities = &m_priority;

                result.push_back(info);
            }

            return result;
        }

        LogicalDevice() = default;

        void InitializeQueues(const PhysicalDeviceInfo& device)
        {
            for (const auto& family : device.queueFamilies)
            {
                if (family.flags & VK_QUEUE_GRAPHICS_BIT && m_graphicsQueue == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(m_device, family.index, 0, &m_graphicsQueue);
                }
                if (family.flags & VK_QUEUE_COMPUTE_BIT && m_computeQueue == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(m_device, family.index, 0, &m_computeQueue);
                }
                if (family.flags & VK_QUEUE_TRANSFER_BIT && m_transferQueue == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(m_device, family.index, 0, &m_transferQueue);
                }
                if (family.present && m_presentQueue == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(m_device, family.index, 0, &m_presentQueue);
                }
            }
        }

        void Init(const PhysicalDeviceInfo& device)
        {
            const auto queueInfos = CalculateQueueCreateInfos(device);

            VkDeviceCreateInfo createInfo{};
            createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueInfos.size());
            createInfo.pQueueCreateInfos       = queueInfos.data();
            createInfo.enabledExtensionCount   = static_cast<uint32_t>(device.extensions.size());
            createInfo.ppEnabledExtensionNames = device.extensions.data();
            createInfo.pEnabledFeatures        = &device.features;

            VulkanChecker::ThrowIfNotSuccess(vkCreateDevice(device.device, &createInfo, nullptr, &m_device));
            InitializeQueues(device);
        }

    public:
        static std::unique_ptr<LogicalDevice> Create(const PhysicalDeviceInfo& device)
        {
            auto result = std::unique_ptr<LogicalDevice>(new LogicalDevice);
            result->Init(device);
            return result;
        }

        ~LogicalDevice()
        {
            vkDestroyDevice(m_device, nullptr);
        }
    };
} // namespace Chess::Engine
