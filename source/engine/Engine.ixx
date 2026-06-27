module;
#include <cstdint>
export module Chess.Engine.Engine;
import Chess.Engine.Instance;
import Chess.Engine.LogicalDevices;
import Chess.Engine.PhysicalDevices;

namespace Chess::Engine
{
    export class Engine
    {
        Instance        m_instance;
        PhysicalDevices m_physicalDevices;
        LogicalDevices  m_logicalDevices;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            m_instance        = Instance::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            m_physicalDevices = PhysicalDevices::Create(m_instance.GetInstance());
            m_logicalDevices  = LogicalDevices::Create(m_physicalDevices.GetPhysicalDevices(), m_physicalDevices.GetPhysicalDevicesFeatures());
        }

    public:
        static Engine Create(
            const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            auto result = Engine();
            result.Init(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            return result;
        }

        const Instance& GetInstance() const
        {
            return m_instance;
        }
    };
} // namespace Chess::Engine
