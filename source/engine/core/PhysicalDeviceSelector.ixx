module;
#include <algorithm>
#include <cstring>
#include <print>
#include <string_view>
#include <vector>
#include <vulkan/vulkan_core.h>
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
        static bool HasQueue(const PhysicalDeviceInfo& device, VkQueueFlagBits flag)
        {
            return std::ranges::any_of(device.queueFamilies, [flag](const auto& family) { return (family.flags & flag) != 0; });
        }

        static bool HasPresentQueue(const PhysicalDeviceInfo& device)
        {
            return std::ranges::any_of(device.queueFamilies, [](const auto& family) { return family.present; });
        }

        static bool SupportsExtension(const PhysicalDeviceInfo& device, std::string_view name)
        {
            return std::ranges::any_of(device.extensions, [name](const auto& extension) { return name == extension.extensionName; });
        }

        static bool IsSuitable(const PhysicalDeviceInfo& device)
        {
            return HasQueue(device, VK_QUEUE_GRAPHICS_BIT) && HasPresentQueue(device)
                && SupportsExtension(device, VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        }

    public:
        static const PhysicalDeviceInfo& Select(const PhysicalDevices& devices)
        {
            std::vector<const PhysicalDeviceInfo*> suitable;
            for (const auto& device : devices.GetDevices())
            {
                if (IsSuitable(device))
                {
                    suitable.push_back(&device);
                }
            }

            if (suitable.empty())
            {
                throw Utils::EngineException("No Vulkan device with graphics, present and swapchain support was found");
            }

            std::println("Available Vulkan devices:");
            for (std::size_t i = 0; i < suitable.size(); ++i)
            {
                std::println("{} : {}", i, suitable[i]->properties.deviceName);
            }
            std::print("\nSelect device: ");

            uint32_t index = 0u;
            while (true)
            {
                auto input     = Utils::ConsoleReader::ReadLine();
                auto converted = Utils::Converter::ToUint32(input);
                if (!converted.has_value() || converted.value() >= suitable.size())
                {
                    continue;
                }
                index = converted.value();
                break;
            }

            return *suitable[index];
        }
    };
} // namespace Chess::Engine
