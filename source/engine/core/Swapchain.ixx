module;
#include <memory>
#include <vector>
#include <vulkan/vulkan_core.h>
export module Chess.Engine.Swapchain;
import Chess.Engine.VulkanChecker;

namespace Chess::Engine
{
    export class Swapchain
    {
        VkDevice       m_device    = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

        Swapchain() = default;

        void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
        {
            m_device = device;

            VkSurfaceCapabilitiesKHR capabilities;
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

            auto createInfo             = VkSwapchainCreateInfoKHR();
            createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface          = surface;
            createInfo.minImageCount    = capabilities.minImageCount;
            createInfo.imageExtent      = capabilities.currentExtent;
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.preTransform     = capabilities.currentTransform;
            createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode      = VK_PRESENT_MODE_FIFO_KHR;
            createInfo.clipped          = VK_TRUE;
            createInfo.oldSwapchain     = VK_NULL_HANDLE;

            uint32_t count = 0;
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr);

            std::vector<VkSurfaceFormatKHR> formats(count);

            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data());

            createInfo.imageFormat     = formats[0].format;
            createInfo.imageColorSpace = formats[0].colorSpace;

            VulkanChecker::ThrowIfNotSuccess(vkCreateSwapchainKHR(device, &createInfo, nullptr, std::addressof(m_swapchain)));
        }

    public:
        static std::unique_ptr<Swapchain> Create(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface)
        {
            auto result = std::unique_ptr<Swapchain>(new Swapchain());
            result->Init(physicalDevice, device, surface);
            return result;
        }

        ~Swapchain()
        {
            if (m_swapchain != VK_NULL_HANDLE)
            {
                vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
            }
        }

        VkSwapchainKHR GetSwapchain() const
        {
            return m_swapchain;
        }
    };
} // namespace Chess::Engine
