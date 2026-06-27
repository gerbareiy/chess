module;
#define GLFW_INCLUDE_VULKAN ;
#include <GLFW/glfw3.h>
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

            m_instance         = Instance::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            m_surface          = Surface::Create(*m_instance, m_window);
            m_physicalDevices  = PhysicalDevices::Create(m_instance->GetInstance(), m_surface->GetSurface());
            const auto& device = PhysicalDeviceSelector::Select(*m_physicalDevices);
            m_logicalDevice    = LogicalDevice::Create(device);
            m_swapchain        = Swapchain::Create(device.device, m_logicalDevice->GetDevice(), m_surface->GetSurface());
        }

    public:
        static Engine Create(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result = Engine();
            result.Init(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            return result;
        }

        Engine(Engine&& other)            = default;
        Engine& operator=(Engine&& other) = default;

        ~Engine()
        {
            if (m_window != nullptr)
            {
                glfwDestroyWindow(m_window);
            }
            glfwTerminate();
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
