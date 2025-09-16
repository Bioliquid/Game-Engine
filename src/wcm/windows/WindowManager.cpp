#include "WindowManager.hpp"

#include "Logger.hpp"

namespace bvd {

void WindowManager::init() {
    instance = GetModuleHandle(nullptr);

    BVD_LOG(LogMod::wcm, LogLvl::dbg, "Initialized window manager");
}

void WindowManager::destroy() {
    // windows.clear();
    while (not windows.empty()) {
        windows.back().destroy();
    }

    BVD_LOG(LogMod::wcm, LogLvl::dbg, "Destroyed window manager");
}

void WindowManager::addWindow(Window& window) {
    windows.push_back(window);
}

void WindowManager::removeWindow(Window& window) {
    windows.erase(windows.iterator_to(window));
}

bool WindowManager::pollEvents() {
    MSG msg;

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            requestToClose();
            return false;
        } else {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    return true;
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
