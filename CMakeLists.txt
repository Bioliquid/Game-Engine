cmake_minimum_required(VERSION 3.22)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

project(biovoid)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# General variables
set(RootDir ${CMAKE_CURRENT_SOURCE_DIR})
set(SrcDir ${RootDir}/src)
set(ExtDir ${RootDir}/externals)

if(${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    set(BuildType release)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(BuildType debug)
endif()

if(WIN32)
    # for Windows operating system in general
    set(BuildPlatform windows)
    set(WindowsPlatform TRUE)
    set(ApplePlatform FALSE)
    set(LinuxPlatform FALSE)
    set(CMAKE_SHARED_LINKER_FLAGS /MANIFEST:NO)
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
elseif(APPLE)
    # for MacOS X or iOS, watchOS, tvOS (since 3.10.3)
    set(BuildPlatform macos)
    set(WindowsPlatform FALSE)
    set(ApplePlatform TRUE)
    set(LinuxPlatform FALSE)
elseif(UNIX AND NOT APPLE)
    # for Linux, BSD, Solaris, Minix
    set(BuildPlatform linux)
    set(WindowsPlatform FALSE)
    set(ApplePlatform FALSE)
    set(LinuxPlatform TRUE)
endif()

# set(UseGlfw TRUE)

# add_compile_options(-fsanitize=address)

set(BUILD_SHARED_LIBS OFF)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(
    BoostAssert
    GIT_REPOSITORY https://github.com/boostorg/assert.git
    GIT_TAG        boost-1.89.0
)
FetchContent_MakeAvailable(BoostAssert)

FetchContent_Declare(
    BoostConfig
    GIT_REPOSITORY https://github.com/boostorg/config.git
    GIT_TAG        boost-1.89.0
)
FetchContent_MakeAvailable(BoostConfig)

FetchContent_Declare(
    BoostMove
    GIT_REPOSITORY https://github.com/boostorg/move.git
    GIT_TAG        boost-1.89.0
)
FetchContent_MakeAvailable(BoostMove)

FetchContent_Declare(
    BoostIntrusive
    GIT_REPOSITORY https://github.com/boostorg/intrusive.git
    GIT_TAG        boost-1.89.0
)
FetchContent_MakeAvailable(BoostIntrusive)

# Attach externals
add_subdirectory(${ExtDir}/fmt)
add_subdirectory(${ExtDir}/magic_enum)
if (${UseGlfw})
    add_subdirectory(${ExtDir}/glfw)
endif()

# Project directories
add_subdirectory(src)
add_subdirectory(editor)
