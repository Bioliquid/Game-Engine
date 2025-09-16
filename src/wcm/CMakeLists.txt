include(CommonOptions)

# windows control manager

if (${UseGlfw})

create_library(wcm
    SRCS
        glfw/Window.cpp
        glfw/WindowManager.cpp
    DIRS
        .
        glfw
    LIBS
        inc::core
        glfw
        Boost::intrusive
)

else()

create_library(wcm
    SRCS
        ${BuildPlatform}/Window.cpp
        ${BuildPlatform}/WindowManager.cpp
    DIRS
        .
        ${BuildPlatform}
    LIBS
        inc::core
        Boost::intrusive
)

endif()
