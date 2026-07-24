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
    public:
        ~Surface()
        {
            if (surface_ != VK_NULL_HANDLE)
            {
                vkDestroySurfaceKHR(instance_, surface_, nullptr);
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
            return surface_;
        }

    private:
        VkInstance   instance_ = VK_NULL_HANDLE;
        VkSurfaceKHR surface_  = VK_NULL_HANDLE;

        static VkSurfaceKHR CalculateSurface(const VkInstance& instance, GLFWwindow* window)
        {
            VkSurfaceKHR result = VK_NULL_HANDLE;
            VulkanChecker::ThrowIfNotSuccess(glfwCreateWindowSurface(instance, window, nullptr, std::addressof(result)));
            return result;
        }

        Surface() = default;

        void Init(const Instance& instance, GLFWwindow* window)
        {
            instance_ = instance.GetInstance();
            surface_  = CalculateSurface(instance_, window);
        }
    };
} // namespace Chess::Engine
