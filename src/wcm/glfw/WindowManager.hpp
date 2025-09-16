#pragma once

#include "Window.hpp"

#include <boost/intrusive/list.hpp>

namespace bvd {

class WindowManager {
    using WindowList = boost::intrusive::list<Window>;

public:
    void init();
    void destroy();

    void addWindow(Window&);
    void removeWindow(Window&);

    void pollEvents() const;
    bool shouldClose() const;

    void requestToClose();

private:
    WindowList windows;
};

} // namespace bvd
