module;
#define GLFW_INCLUDE_VULKAN ;
#include <GLFW/glfw3.h>
#include <memory>
export module Chess.Engine.Engine;
import Chess.Engine.Context;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class Engine
    {
        GLFWwindow*              m_window = nullptr;
        std::unique_ptr<Context> m_context;
        VkSurfaceKHR             m_surface = VkSurfaceKHR();

        Engine() = default;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            if (!glfwInit())
            {
                throw Utils::EngineException("Couldn't init glfw");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            m_window  = glfwCreateWindow(1600, 900, applicationName, nullptr, nullptr);
            m_context = Context::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            glfwCreateWindowSurface(m_context->GetInstance().GetInstance(), m_window, nullptr, std::addressof(m_surface));
        }

    public:
        static Engine Create(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result = Engine();
            result.Init(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            return result;
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
