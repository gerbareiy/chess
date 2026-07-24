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
    public:
        static std::unique_ptr<LogicalDevice> Create(const PhysicalDeviceInfo& device)
        {
            auto result = std::unique_ptr<LogicalDevice>(new LogicalDevice);
            result->Init(device);
            return result;
        }

        ~LogicalDevice()
        {
            vkDestroyDevice(device_, nullptr);
        }

        const VkDevice& GetDevice() const
        {
            return device_;
        }

    private:
        static constexpr float      priority_   = 1.f;
        static constexpr std::array extensions_ = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        VkDevice device_ = VK_NULL_HANDLE;

        VkQueue graphicsQueue_ = VK_NULL_HANDLE;
        VkQueue presentQueue_  = VK_NULL_HANDLE;
        VkQueue computeQueue_  = VK_NULL_HANDLE;
        VkQueue transferQueue_ = VK_NULL_HANDLE;

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
                info.pQueuePriorities = &priority_;

                result.push_back(info);
            }

            return result;
        }

        LogicalDevice() = default;

        void InitializeQueues(const PhysicalDeviceInfo& device)
        {
            for (const auto& family : device.queueFamilies)
            {
                if (family.flags & VK_QUEUE_GRAPHICS_BIT && graphicsQueue_ == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(device_, family.index, 0, &graphicsQueue_);
                }
                if (family.flags & VK_QUEUE_COMPUTE_BIT && computeQueue_ == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(device_, family.index, 0, &computeQueue_);
                }
                if (family.flags & VK_QUEUE_TRANSFER_BIT && transferQueue_ == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(device_, family.index, 0, &transferQueue_);
                }
                if (family.present && presentQueue_ == VK_NULL_HANDLE)
                {
                    vkGetDeviceQueue(device_, family.index, 0, &presentQueue_);
                }
            }
        }

        void Init(const PhysicalDeviceInfo& device)
        {
            const auto queueInfos = CalculateQueueCreateInfos(device);

            auto createInfo                    = VkDeviceCreateInfo();
            createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueInfos.size());
            createInfo.pQueueCreateInfos       = queueInfos.data();
            createInfo.enabledExtensionCount   = extensions_.size();
            createInfo.ppEnabledExtensionNames = extensions_.data();
            createInfo.pEnabledFeatures        = &device.features;

            VulkanChecker::ThrowIfNotSuccess(vkCreateDevice(device.device, &createInfo, nullptr, std::addressof(device_)));
            InitializeQueues(device);
        }
    };
} // namespace Chess::Engine
