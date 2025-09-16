include_guard(GLOBAL)

function(set_build_option targetName)
    if(${WindowsPlatform})
        target_compile_options(${targetName} PRIVATE 
            #-Wall
            #-Werror
            #-Wno-error=maybe-uninitialized
            #-std=c++2a
            #-save-temps
            /Zc:preprocessor
            #$<$<CONFIG:RELEASE>:-Ofast>
            #$<$<CONFIG:RELEASE>:-funroll-loops> #optimization, probably should be only for release target
            )

        target_compile_options(${targetName} PRIVATE
           -W4
           #-Wall
           -WX
           -wd4061 # enumerator in switch of enum is not explicitly handled by a case label
           -wd4514 # unreferenced inline function has been removed
           -wd4820 # n bytes padding added after data member
           -wd4251 # "needs to have dll-interface to be used by clients of class"
           -wd4710 # function not inlined
           -wd4711 # selected for automatic inline expansion
           -wd5045 # Compiler will insert Spectre mitigation for memory load
           -wd4701 # Potentially uninitialized local variable
           -wd4996
        )
    elseif(${LinuxPlatform})
        target_compile_options(${targetName} PRIVATE 
            -Wall
            -Werror
            #-Wno-error=maybe-uninitialized
            -std=c++2a
            -save-temps
            $<$<CONFIG:RELEASE>:-Ofast>
            $<$<CONFIG:RELEASE>:-funroll-loops> #optimization, probably should be only for release target
            $<$<CONFIG:DEBUG>:-O0>
            $<$<CONFIG:DEBUG>:-ggdb3>
            $<$<CONFIG:DEBUG>:-fstack-protector-all>
            #-fsanitize=address
            #-fsanitize=undefined
        )

        target_compile_options(${targetName} PRIVATE
           -Wall
           -Wextra
        )
    endif()

    target_compile_definitions(${targetName} PRIVATE
        #$<$<CONFIG:Debug>:ENABLE_MEMORY_LEAK_DETECTION>
        USE_VALIDATION_LAYERS
        VULKAN_USE_IMAGE_ACQUIRE_FENCES=true
        DEVELOPMENT_GUIDES_CHECK
        ENABLE_INTERNAL_DEBUG=true
        BOOST_ENABLE_ASSERT_HANDLER
    )
endfunction()

function(create_library targetName)
    set(options SHARED)
    set(oneValueList)
    set(multiValueList SRCS DIRS LIBS PVT_LIBS DEFS)
    cmake_parse_arguments(PROPERTY "${options}" "${oneValueList}" "${multiValueList}" ${ARGN})

    add_library(inc_${targetName} INTERFACE)
    add_library(inc::${targetName} ALIAS inc_${targetName})

    if(DEFINED PROPERTY_DIRS)
        target_include_directories(inc_${targetName} INTERFACE ${PROPERTY_DIRS})
    endif()

    if(DEFINED PROPERTY_SRCS)
        if(${PROPERTY_SHARED})
            add_library(${targetName} SHARED ${PROPERTY_SRCS})
        else()
            add_library(${targetName} ${PROPERTY_SRCS})
        endif()

        set_build_option(${targetName})

        target_link_libraries(${targetName} PUBLIC inc::${targetName} ${PROPERTY_LIBS} PRIVATE ${PROPERTY_PVT_LIBS})

        if(DEFINED PROPERTY_DEFS)
            target_compile_definitions(${targetName} PRIVATE ${PROPERTY_DEFS})
        endif()
    elseif(DEFINED PROPERTY_LIBS)
        target_link_libraries(inc_${targetName} INTERFACE ${PROPERTY_LIBS})
    endif()
endfunction()

function(create_binary targetName)
    set(oneValueList)
    set(multiValueList SRCS DIRS LIBS PVT_LIBS DEFS)
    cmake_parse_arguments(PROPERTY "${options}" "${oneValueList}" "${multiValueList}" ${ARGN})

    add_executable(${targetName} ${PROPERTY_SRCS})

    set_build_option(${targetName})

    if(DEFINED PROPERTY_DIRS)
        target_include_directories(${targetName} INTERFACE ${PROPERTY_DIRS})
    endif()

    target_link_libraries(${targetName} PUBLIC ${PROPERTY_LIBS} PRIVATE ${PROPERTY_PVT_LIBS})

    if(DEFINED PROPERTY_DEFS)
        target_compile_definitions(${targetName} PRIVATE ${PROPERTY_DEFS})
    endif()
endfunction()
