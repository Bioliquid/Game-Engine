#pragma once

#include <boost/intrusive/list_hook.hpp>
#include <string_view>

struct GLFWwindow;

namespace bvd {

class WindowManager;

class Window : public boost::intrusive::list_base_hook<> {
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

    void requestToClose();

    bool shouldClose() const;
    bool isMain() const { return main; }

private:
    WindowManager& manager;

    GLFWwindow* handle;
    bool        main;
};

} // namespace bvd
