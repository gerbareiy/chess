module;
#include <print>
export module Chess.Engine.PhysicalDeviceSelector;
import Chess.Engine.PhysicalDeviceInfo;
import Chess.Engine.PhysicalDevices;
import Chess.Utils.ConsoleReader;
import Chess.Utils.Converter;
import Chess.Utils.Exceptions;

namespace Chess::Engine
{
    export class PhysicalDeviceSelector
    {
    public:
        static const PhysicalDeviceInfo& Select(const PhysicalDevices& devices)
        {
            const auto& list = devices.GetDevices();

            std::println("Available Vulkan devices:");
            for (std::size_t i = 0; i < list.size(); ++i)
            {
                std::println("{} : {}", i, list[i].properties.deviceName);
            }
            std::print("\nSelect device: ");

            uint32_t index = 0u;
            while (true)
            {
                auto input     = Utils::ConsoleReader::ReadLine();
                auto converted = Utils::Converter::ToUint32(input);
                if (!converted.has_value() || converted.value() >= list.size())
                {
                    continue;
                }
                index = converted.value();
                break;
            }

            return list[index];
        }
    };
} // namespace Chess::Engine
