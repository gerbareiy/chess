module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include <utility>
export module Chess.Engine.Engine;
import Chess.Engine.Instance;
import Chess.Engine.LogicalDevice;
import Chess.Engine.PhysicalDevices;
import Chess.Engine.PhysicalDeviceSelector;
import Chess.Engine.Surface;
import Chess.Engine.Swapchain;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class Engine
    {
    public:
        static Engine Create(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result = Engine();
            result.Init(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            return result;
        }

        Engine(const Engine&)            = delete;
        Engine& operator=(const Engine&) = delete;

        Engine(Engine&& other) noexcept
            : instance_(std::move(other.instance_))
            , surface_(std::move(other.surface_))
            , physicalDevices_(std::move(other.physicalDevices_))
            , logicalDevice_(std::move(other.logicalDevice_))
            , swapchain_(std::move(other.swapchain_))
            , window_(std::exchange(other.window_, nullptr))
        {
        }

        Engine& operator=(Engine&& other) noexcept
        {
            if (this != std::addressof(other))
            {
                Destroy();

                instance_        = std::move(other.instance_);
                surface_         = std::move(other.surface_);
                physicalDevices_ = std::move(other.physicalDevices_);
                logicalDevice_   = std::move(other.logicalDevice_);
                swapchain_       = std::move(other.swapchain_);
                window_          = std::exchange(other.window_, nullptr);
            }
            return *this;
        }

        ~Engine()
        {
            Destroy();
        }

        void Update()
        {
            glfwPollEvents();
        }

        bool NeedUpdate() const
        {
            return glfwWindowShouldClose(window_) == 0;
        }

    private:
        std::unique_ptr<Instance>        instance_;
        std::unique_ptr<Surface>         surface_;
        std::unique_ptr<PhysicalDevices> physicalDevices_;
        std::unique_ptr<LogicalDevice>   logicalDevice_;
        std::unique_ptr<Swapchain>       swapchain_;
        GLFWwindow*                      window_ = nullptr;

        Engine() = default;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            if (!glfwInit())
            {
                throw Utils::EngineException("Couldn't init glfw");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            window_ = glfwCreateWindow(1600, 900, applicationName, nullptr, nullptr);
            if (window_ == nullptr)
            {
                glfwTerminate();
                throw Utils::EngineException("Couldn't create window");
            }

            instance_          = Instance::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            surface_           = Surface::Create(*instance_, window_);
            physicalDevices_   = PhysicalDevices::Create(instance_->GetInstance(), surface_->GetSurface());
            const auto& device = PhysicalDeviceSelector::Select(*physicalDevices_);
            logicalDevice_     = LogicalDevice::Create(device);
            swapchain_         = Swapchain::Create(device.device, logicalDevice_->GetDevice(), surface_->GetSurface(), window_);
        }

        void Destroy() noexcept
        {
            swapchain_.reset();
            logicalDevice_.reset();
            physicalDevices_.reset();
            surface_.reset();
            instance_.reset();

            if (window_ != nullptr)
            {
                glfwDestroyWindow(window_);
                window_ = nullptr;
                glfwTerminate();
            }
        }
    };
} // namespace Chess::Engine
