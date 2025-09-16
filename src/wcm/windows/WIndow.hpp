#pragma once

#define OEMRESOURCE
#include <Windows.h>
#include <boost/intrusive/list_hook.hpp>
#include <cstdint>
#include <string_view>
#include <string>

namespace bvd {

class WindowManager;

class Window : public boost::intrusive::list_base_hook<> {
    static constexpr std::wstring_view lpszClassName = L"Engine Class";

public:
    static constexpr std::wstring_view lpContextName = L"Engine Context";

    struct Init {
        std::string_view title;
        int32_t          width;
        int32_t          height;
        bool             maximized{false};
        bool             main{false};
        bool             decorated{true};
        bool             resizable{true};
    };

public:
    Window(WindowManager&);

    void init(Init const&);
    void destroy();

    void setWidth(int32_t newWidth) { width = newWidth; }
    void setHeight(int32_t newHeight) { height = newHeight; }
    void setMaximized(bool newMaximized) { maximized = newMaximized; }
    void requestToClose() { closeRequested = true; }

    int32_t getWidth() const { return width; }
    int32_t getHeight() const { return height; }
    bool    shouldClose() const { return closeRequested; }
    bool    isMaximized() const { return maximized; }
    bool    isMain() const { return main; }

private:
    DWORD getWindowStyle() const;
    DWORD getWindowExStyle() const;

private:
    WindowManager& manager;

    HWND         handle;
    std::wstring title;
    int32_t      width;
    int32_t      height;
    bool         maximized;
    bool         main;
    bool         decorated;
    bool         resizable;
    bool         closeRequested;
};

} // namespace bvd
