module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdint>
#include <limits>
#include <memory>
#include <vector>
export module Chess.Engine.Swapchain;
import Chess.Engine.VulkanChecker;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class Swapchain
    {
        VkDevice       m_device    = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;

        Swapchain() = default;

        static std::vector<VkSurfaceFormatKHR> QuerySurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
        {
            uint32_t count = 0;
            VulkanChecker::ThrowIfNotSuccess(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, nullptr));
            if (count == 0)
            {
                throw Utils::EngineException("Surface reports no supported formats");
            }
            std::vector<VkSurfaceFormatKHR> formats(count);
            VulkanChecker::ThrowIfNotSuccess(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &count, formats.data()));
            return formats;
        }

        static VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
        {
            for (const auto& format : formats)
            {
                if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                {
                    return format;
                }
            }
            return formats.front();
        }

        static uint32_t ChooseImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
        {
            uint32_t count = capabilities.minImageCount + 1;
            if (capabilities.maxImageCount > 0 && count > capabilities.maxImageCount)
            {
                count = capabilities.maxImageCount;
            }
            return count;
        }

        static VkExtent2D ChooseExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
        {
            if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            {
                return capabilities.currentExtent;
            }

            int width  = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);

            VkExtent2D extent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
            extent.width  = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            return extent;
        }

        void Init(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window)
        {
            m_device = device;

            VkSurfaceCapabilitiesKHR capabilities{};
            VulkanChecker::ThrowIfNotSuccess(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities));

            const auto formats       = QuerySurfaceFormats(physicalDevice, surface);
            const auto surfaceFormat = ChooseSurfaceFormat(formats);

            auto createInfo             = VkSwapchainCreateInfoKHR();
            createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
            createInfo.surface          = surface;
            createInfo.minImageCount    = ChooseImageCount(capabilities);
            createInfo.imageFormat      = surfaceFormat.format;
            createInfo.imageColorSpace  = surfaceFormat.colorSpace;
            createInfo.imageExtent      = ChooseExtent(capabilities, window);
            createInfo.imageArrayLayers = 1;
            createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.preTransform     = capabilities.currentTransform;
            createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
            createInfo.presentMode      = VK_PRESENT_MODE_FIFO_KHR;
            createInfo.clipped          = VK_TRUE;
            createInfo.oldSwapchain     = VK_NULL_HANDLE;

            VulkanChecker::ThrowIfNotSuccess(vkCreateSwapchainKHR(device, &createInfo, nullptr, std::addressof(m_swapchain)));
        }

    public:
        static std::unique_ptr<Swapchain> Create(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window)
        {
            auto result = std::unique_ptr<Swapchain>(new Swapchain());
            result->Init(physicalDevice, device, surface, window);
            return result;
        }

        Swapchain(const Swapchain&)            = delete;
        Swapchain& operator=(const Swapchain&) = delete;

        Swapchain(Swapchain&&)            = delete;
        Swapchain& operator=(Swapchain&&) = delete;

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
