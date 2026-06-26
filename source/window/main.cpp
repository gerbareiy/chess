#define GLFW_INCLUDE_VULKAN ;
#include <GLFW/glfw3.h>
#include <ranges>
#include <vector>
import Chess.Window.AppInfo;

int main()
{
    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto* window = glfwCreateWindow(1600, 900, Chess::Window::AppInfo::APP_NAME.data(), nullptr, nullptr);

    uint32_t     count          = 0u;
    const char** charExtensions = glfwGetRequiredInstanceExtensions(&count);
    const auto   extensions     = std::vector(charExtensions, charExtensions + count);

    auto info               = VkApplicationInfo();
    info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    info.pApplicationName   = Chess::Window::AppInfo::APP_NAME.data();
    info.applicationVersion = Chess::Window::AppInfo::APP_VERSION;
    info.pEngineName        = Chess::Window::AppInfo::ENGINE_NAME.data();
    info.engineVersion      = Chess::Window::AppInfo::ENGINE_VERSION;
    info.apiVersion         = Chess::Window::AppInfo::API_VERSION;

    auto createInfo                    = VkInstanceCreateInfo();
    createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo        = &info;
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    auto instance                      = VkInstance();

    vkCreateInstance(&createInfo, nullptr, &instance);
    while (glfwWindowShouldClose(window) == 0)
    {
        glfwPollEvents();
    }

    return 0;
}
