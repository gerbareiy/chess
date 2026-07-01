module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <utility>
#include <memory>
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
        std::unique_ptr<Instance>        m_instance;
        std::unique_ptr<Surface>         m_surface;
        std::unique_ptr<PhysicalDevices> m_physicalDevices;
        std::unique_ptr<LogicalDevice>   m_logicalDevice;
        std::unique_ptr<Swapchain>       m_swapchain;
        GLFWwindow*                      m_window = nullptr;

        Engine() = default;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            if (!glfwInit())
            {
                throw Utils::EngineException("Couldn't init glfw");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            m_window = glfwCreateWindow(1600, 900, applicationName, nullptr, nullptr);
            if (m_window == nullptr)
            {
                glfwTerminate();
                throw Utils::EngineException("Couldn't create window");
            }

            m_instance         = Instance::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            m_surface          = Surface::Create(*m_instance, m_window);
            m_physicalDevices  = PhysicalDevices::Create(m_instance->GetInstance(), m_surface->GetSurface());
            const auto& device = PhysicalDeviceSelector::Select(*m_physicalDevices);
            m_logicalDevice    = LogicalDevice::Create(device);
            m_swapchain        = Swapchain::Create(device.device, m_logicalDevice->GetDevice(), m_surface->GetSurface(), m_window);
        }

        void Destroy() noexcept
        {
            // Vulkan objects must be released before the window / glfw is torn down.
            m_swapchain.reset();
            m_logicalDevice.reset();
            m_physicalDevices.reset();
            m_surface.reset();
            m_instance.reset();

            if (m_window != nullptr)
            {
                glfwDestroyWindow(m_window);
                m_window = nullptr;
                glfwTerminate();
            }
        }

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
            : m_instance(std::move(other.m_instance))
            , m_surface(std::move(other.m_surface))
            , m_physicalDevices(std::move(other.m_physicalDevices))
            , m_logicalDevice(std::move(other.m_logicalDevice))
            , m_swapchain(std::move(other.m_swapchain))
            , m_window(std::exchange(other.m_window, nullptr))
        {
        }

        Engine& operator=(Engine&& other) noexcept
        {
            if (this != std::addressof(other))
            {
                Destroy();

                m_instance        = std::move(other.m_instance);
                m_surface         = std::move(other.m_surface);
                m_physicalDevices = std::move(other.m_physicalDevices);
                m_logicalDevice   = std::move(other.m_logicalDevice);
                m_swapchain       = std::move(other.m_swapchain);
                m_window          = std::exchange(other.m_window, nullptr);
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
            return glfwWindowShouldClose(m_window) == 0;
        }
    };
} // namespace Chess::Engine
