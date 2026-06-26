module;
#include <string>
#include <vulkan/vulkan_core.h>
export module Chess.Window.AppInfo;

namespace Chess::Window::AppInfo
{
    export constexpr std::string_view APP_NAME    = "Chess";
    export constexpr uint32_t         APP_VERSION = VK_MAKE_VERSION(0, 0, 1);

    export constexpr std::string_view ENGINE_NAME    = "Chess Engine";
    export constexpr uint32_t         ENGINE_VERSION = VK_MAKE_VERSION(0, 0, 1);

    export constexpr uint32_t API_VERSION = VK_API_VERSION_1_4;
} // namespace Chess::Window::AppInfo
