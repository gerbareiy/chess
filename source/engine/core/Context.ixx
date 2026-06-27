module;
#include <cstdint>
#include <memory>
export module Chess.Engine.Context;
import Chess.Engine.Instance;
import Chess.Engine.LogicalDevice;
import Chess.Engine.PhysicalDevices;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class Context
    {
        std::unique_ptr<Instance>        m_instance;
        std::unique_ptr<PhysicalDevices> m_physicalDevices;
        std::unique_ptr<LogicalDevice>   m_logicalDevice;

        Context() = default;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            m_instance = Instance::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            if (m_instance == nullptr)
            {
                throw Utils::EngineException("Instance is nullptr");
            }

            m_physicalDevices = PhysicalDevices::Create(m_instance->GetInstance());
            if (m_physicalDevices == nullptr)
            {
                throw Utils::EngineException("Physical Devices is nullptr");
            }

            m_logicalDevice = LogicalDevice::Create(m_physicalDevices->GetPhysicalDevices(), m_physicalDevices->GetPhysicalDevicesFeatures());
            if (m_logicalDevice == nullptr)
            {
                throw Utils::EngineException("Logical Device is nullptr");
            }
        }

    public:
        static std::unique_ptr<Context> Create(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result = std::unique_ptr<Context>(new Context);
            result->Init(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            return result;
        }

        const Instance& GetInstance() const
        {
            if (m_instance == nullptr)
            {
                throw Utils::EngineException("Instance is nullptr");
            }
            return *m_instance;
        }

        const LogicalDevice& GetLogicalDevice() const
        {
            if (m_logicalDevice == nullptr)
            {
                throw Utils::EngineException("Logical Device is nullptr");
            }
            return *m_logicalDevice;
        }
    };
} // namespace Chess::Engine
