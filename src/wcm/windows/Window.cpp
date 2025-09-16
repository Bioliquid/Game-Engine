#include "Window.hpp"

#include "WindowManager.hpp"
#include "Logger.hpp"

namespace bvd {

std::wstring createWideStringFromUTF8Win32(char const* source) {
    int count = MultiByteToWideChar(CP_UTF8, 0, source, -1, NULL, 0);
    if (count == 0) {
        BVD_LOG(LogMod::wcm, LogLvl::err, "Failed to convert string from UTF-8");
        return L"";
    }

    std::wstring target;
    target.reserve(count);

    if (not MultiByteToWideChar(CP_UTF8, 0, source, -1, target.data(), count)) {
        BVD_LOG(LogMod::wcm, LogLvl::err, "Failed to convert string from UTF-8");
        return L"";
    }

    return target;
}

static LRESULT CALLBACK windowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window* window = static_cast<Window*>(GetPropW(hWnd, Window::lpContextName.data()));
    if (window == nullptr) {
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    switch (uMsg) {
    case WM_CLOSE: {
        // window->requestToClose();
        window->destroy();
        return 0;
    }
    case WM_DESTROY: {
        if (window->isMain()) {
            PostQuitMessage(0);
            return 0;
        }
    }
    case WM_SIZE: {
        int const  width = LOWORD(lParam);
        int const  height = HIWORD(lParam);
        bool const maximized = wParam == SIZE_MAXIMIZED or (window->isMaximized() and wParam != SIZE_RESTORED);

        if (width != window->getWidth() || height != window->getHeight()) {
            window->setWidth(width);
            window->setHeight(height);
        }

        window->setMaximized(maximized);
        return 0;
    }
    }

    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}


Window::Window(WindowManager& wm) : manager(wm), maximized(false), closeRequested(false) {}

void Window::init(Init const& init) {
    width     = init.width;
    height    = init.height;
    maximized = init.maximized;
    main      = init.main;
    decorated = init.decorated;
    resizable = init.resizable;

    DWORD style = getWindowStyle();
    DWORD exStyle = getWindowExStyle();

    WNDCLASSEXW wc = { sizeof(wc) };
    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = windowProc;
    wc.hInstance     = manager.getHandle();
    wc.lpszClassName = lpszClassName.data();
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);

    RegisterClassExW(&wc);

    RECT rect = { 250, 250, init.width, init.height };

    if (init.maximized) {
        style |= WS_MAXIMIZE;
    }

    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    title = createWideStringFromUTF8Win32(init.title.data());

    handle = CreateWindowExW(0, lpszClassName.data(), title.c_str(), style, rect.left, rect.top, rect.right - rect.left,
                             rect.bottom - rect.top, NULL, NULL, manager.getHandle(), NULL);

    SetPropW(handle, lpContextName.data(), this);

    ShowWindow(handle, SW_SHOW);

    manager.addWindow(*this);

    BVD_LOG(LogMod::wcm, LogLvl::inf, "Initialized window title=\"{}\" width={} height={}", init.title, init.width, init.height);
}

void Window::destroy() {
    manager.removeWindow(*this);

    DestroyWindow(handle);

    UnregisterClassW(lpszClassName.data(), manager.getHandle());

    BVD_LOG(LogMod::wcm, LogLvl::dbg, "Destroyed window");
}

DWORD Window::getWindowStyle() const {
    DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    constexpr bool monitor = false;

    if (monitor) {
        style |= WS_POPUP;
    } else {
        style |= WS_SYSMENU | WS_MINIMIZEBOX;

        if (decorated) {
            style |= WS_CAPTION;

            if (resizable) {
                style |= WS_MAXIMIZEBOX | WS_THICKFRAME;
            }
        } else {
            style |= WS_POPUP;
        }
    }

    return style;
}

DWORD Window::getWindowExStyle() const {
    DWORD style = WS_EX_APPWINDOW;

    constexpr bool monitor = false;
    constexpr bool floating = false;

    if constexpr (monitor or floating) {
        style |= WS_EX_TOPMOST;
    }

    return style;
}

} // namespace bvd
