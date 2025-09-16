#include "WindowManager.hpp"

#include "Logger.hpp"

#include <glfw/glfw3.h>

namespace bvd {

void WindowManager::init() {
    if (glfwInit() == GLFW_FALSE) {
        BVD_THROW("Failed to initialize glfw");
    }

    BVD_LOG(LogMod::wcm, LogLvl::dbg, "Initialized window manager");
}

void WindowManager::destroy() {
    // windows.clear();
    while (not windows.empty()) {
        windows.back().destroy();
    }

    glfwTerminate();

    BVD_LOG(LogMod::wcm, LogLvl::dbg, "Destroyed window manager");
}

void WindowManager::addWindow(Window& window) {
    windows.push_back(window);
}

void WindowManager::removeWindow(Window& window) {
    windows.erase(windows.iterator_to(window));
}

void WindowManager::pollEvents() const {
    glfwPollEvents();
}

bool WindowManager::shouldClose() const {
    return std::ranges::all_of(windows, &Window::shouldClose);
}

void WindowManager::requestToClose() {
    for (Window& window : windows) {
        window.requestToClose();
    }
}

} // namespace bvd
