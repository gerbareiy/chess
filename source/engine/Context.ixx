module;
#include <cstdint>
#include <memory>
export module Chess.Engine.Context;
import Chess.Engine.Instance;
import Chess.Engine.LogicalDevices;
import Chess.Engine.PhysicalDevices;

namespace Chess::Engine
{
    export class Context
    {
        std::unique_ptr<Instance>        m_instance;
        std::unique_ptr<PhysicalDevices> m_physicalDevices;
        std::unique_ptr<LogicalDevices>  m_logicalDevices;

        Context() = default;

        void Init(const char* applicationName, uint32_t applicationVersion, const char* engineName, uint32_t engineVersion, uint32_t apiVersion)
        {
            m_instance        = Instance::Create(applicationName, applicationVersion, engineName, engineVersion, apiVersion);
            m_physicalDevices = PhysicalDevices::Create(m_instance->GetInstance());
            m_logicalDevices  = LogicalDevices::Create(m_physicalDevices->GetPhysicalDevices(), m_physicalDevices->GetPhysicalDevicesFeatures());
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
            return *m_instance.get();
        }
    };
} // namespace Chess::Engine
