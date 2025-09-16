#include "Window.hpp"
#include "WindowManager.hpp"
#include "Logger.hpp"

#include <boost/assert.hpp>

namespace boost
{

void assertion_failed(char const* expr, char const*, char const* file, long line) {
    BVD_LOG(bvd::LogMod::gen, bvd::LogLvl::err, "{}, file {}, line {}", expr, file, line);
}

} // namespace boost


using namespace bvd;

int main() {
    WindowManager windowManager;
    windowManager.init();

    Window window1(windowManager);
    window1.init({.title = "Game Engine 1", .width = 1920, .height = 1080, .main = true});

    Window window2(windowManager);
    window2.init({.title = "Game Engine 2", .width = 1280, .height = 720, .main = false});

    while (not windowManager.shouldClose()) {
        windowManager.pollEvents();
    }

    windowManager.destroy();
}
