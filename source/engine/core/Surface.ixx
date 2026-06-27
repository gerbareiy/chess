module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
export module Chess.Engine.Surface;
import Chess.Engine.Instance;
import Chess.Engine.VulkanChecker;

namespace Chess::Engine
{
    export class Surface
    {
        VkInstance   m_instance = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface  = VK_NULL_HANDLE;

        static VkSurfaceKHR CalculateSurface(const VkInstance& instance, GLFWwindow* window)
        {
            VkSurfaceKHR result = VK_NULL_HANDLE;
            VulkanChecker::ThrowIfNotSuccess(glfwCreateWindowSurface(instance, window, nullptr, std::addressof(result)));
            return result;
        }

        Surface() = default;

        void Init(const Instance& instance, GLFWwindow* window)
        {
            m_instance = instance.GetInstance();
            m_surface  = CalculateSurface(m_instance, window);
        }

    public:
        ~Surface()
        {
            if (m_surface != VK_NULL_HANDLE)
            {
                vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
            }
        }

        Surface(const Surface&)            = delete;
        Surface& operator=(const Surface&) = delete;

        Surface(Surface&&)            = delete;
        Surface& operator=(Surface&&) = delete;

        static std::unique_ptr<Surface> Create(const Instance& instance, GLFWwindow* window)
        {
            auto result = std::unique_ptr<Surface>(new Surface);
            result->Init(instance, window);
            return result;
        }

        VkSurfaceKHR GetSurface() const
        {
            return m_surface;
        }
    };
} // namespace Chess::Engine
