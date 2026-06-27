module;
#define GLFW_INCLUDE_VULKAN ;
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
export module Chess.Engine.Instance;
import Chess.Engine.VulkanChecker;

namespace Chess::Engine
{
    export class Instance
    {
        std::vector<const char*> m_extensions;
        VkApplicationInfo        m_info       = VkApplicationInfo();
        VkInstanceCreateInfo     m_createInfo = VkInstanceCreateInfo();
        VkInstance               m_instance   = VkInstance();

        static std::vector<const char*> CalculateExtensions()
        {
            uint32_t     count      = 0u;
            const char** extensions = glfwGetRequiredInstanceExtensions(std::addressof(count));
            return std::vector(extensions, extensions + count);
        }

        static VkApplicationInfo CalculateApplicationInfo(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result               = VkApplicationInfo();
            result.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            result.pApplicationName   = applicationName;
            result.applicationVersion = applicationVersion;
            result.pEngineName        = engineName;
            result.engineVersion      = engineVersion;
            result.apiVersion         = apiVersion;
            return result;
        }

        static VkInstanceCreateInfo CalculateInstanceCreateInfo(const std::vector<const char*>& extensions, const VkApplicationInfo& info)
        {
            auto result                    = VkInstanceCreateInfo();
            result.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            result.pApplicationInfo        = std::addressof(info);
            result.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
            result.ppEnabledExtensionNames = extensions.data();
            return result;
        }

        static VkInstance CalculateInstance(const VkInstanceCreateInfo& info)
        {
            auto result = VkInstance();
            VulkanChecker::ThrowIfNotSuccess(vkCreateInstance(std::addressof(info), nullptr, std::addressof(result)));
            return result;
        }

        Instance() = default;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            m_extensions = CalculateExtensions();
            m_info       = CalculateApplicationInfo(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            m_createInfo = CalculateInstanceCreateInfo(m_extensions, m_info);
            m_instance   = CalculateInstance(m_createInfo);
        }

    public:
        static std::unique_ptr<Instance> Create(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result = std::unique_ptr<Instance>(new Instance);
            result->Init(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            return result;
        }

        const VkInstance& GetInstance() const
        {
            return m_instance;
        }
    };
} // namespace Chess::Engine
