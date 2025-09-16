include(CommonOptions)

create_library(core
    DIRS
        .
    LIBS
        fmt
        magic_enum::magic_enum
)
