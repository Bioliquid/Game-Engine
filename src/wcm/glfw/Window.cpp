#include "Window.hpp"

#include "WindowManager.hpp"
#include "Logger.hpp"

#include <glfw/glfw3.h>

namespace bvd {

Window::Window(WindowManager& windowManager) : manager(windowManager) {}

void Window::init(Init const& init) {
    main = init.main;

    glfwWindowHint(GLFW_MAXIMIZED, init.maximized ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, init.decorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, init.resizable ? GLFW_TRUE : GLFW_FALSE);

    handle = glfwCreateWindow(init.width, init.height, init.title.data(), nullptr, nullptr);

    if (!handle) {
        BVD_THROW("Failed to create a window");
    }

    glfwSetWindowUserPointer(handle, this);

    glfwSetWindowCloseCallback(handle, [](GLFWwindow* handle) {
        Window& window = *reinterpret_cast<Window*>(glfwGetWindowUserPointer(handle));
        if (window.isMain()) {
            window.manager.requestToClose();
        } else {
            window.destroy();
        }
    });

    manager.addWindow(*this);

    BVD_LOG(LogMod::wcm, LogLvl::inf, "Initialized window title=\"{}\" width={} height={}", init.title, init.width, init.height);
}

void Window::destroy() {
    manager.removeWindow(*this);

    glfwDestroyWindow(handle);

    BVD_LOG(LogMod::wcm, LogLvl::dbg, "Destroyed window");
}

void Window::requestToClose() {
    glfwSetWindowShouldClose(handle, GLFW_TRUE);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(handle);
}

} // namespace bvd
