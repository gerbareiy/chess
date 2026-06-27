#define GLFW_INCLUDE_VULKAN ;
#include <GLFW/glfw3.h>
#include <ranges>
import Chess.Engine.Engine;
import Chess.Window.AppInfo;

int main()
{
    if (!glfwInit())
    {
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    auto* window = glfwCreateWindow(1600, 900, Chess::Window::AppInfo::APP_NAME.data(), nullptr, nullptr);

    const auto engine = Chess::Engine::Engine::Create(
        Chess::Window::AppInfo::APP_NAME.data(),
        Chess::Window::AppInfo::APP_VERSION,
        Chess::Window::AppInfo::ENGINE_NAME.data(),
        Chess::Window::AppInfo::ENGINE_VERSION,
        Chess::Window::AppInfo::API_VERSION);

    auto surface = VkSurfaceKHR();
    glfwCreateWindowSurface(engine.GetInstance().GetInstance(), window, nullptr, &surface);

    while (glfwWindowShouldClose(window) == 0)
    {
        glfwPollEvents();
    }

    return 0;
}
