#pragma once

#include "Window.hpp"

#define OEMRESOURCE
#include <Windows.h>
#include <boost/intrusive/list.hpp>

namespace bvd {

class WindowManager {
    using WindowList = boost::intrusive::list<Window>;

public:
    void init();
    void destroy();

    void addWindow(Window&);
    void removeWindow(Window&);

    bool pollEvents();
    bool shouldClose() const;

    void requestToClose();

    HINSTANCE getHandle() const { return instance; }

private:
    HINSTANCE  instance;
    WindowList windows;
};

} // namespace bvd
