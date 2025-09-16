#pragma once

#include "FsDefs.hpp"

#include <fmt/ranges.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <magic_enum/magic_enum.hpp>
#include <array>

namespace bvd {

enum class LogMod {
    gen = 0,
    wcm
};

enum class LogLvl {
    dbg,
    inf,
    wrn,
    err,
    vip,
    ftl
};

inline std::array<LogLvl, magic_enum::enum_count<LogMod>()> logLevels;

constexpr std::string_view getLogLvlName(bvd::LogLvl lvl) {
    switch (lvl)
    {
    case LogLvl::dbg: return "DBG";
    case LogLvl::inf: return "INF";
    case LogLvl::wrn: return "WRN";
    case LogLvl::err: return "ERR";
    case LogLvl::vip: return "VIP";
    case LogLvl::ftl: return "FTL";
    }
    return "unknown";
}

constexpr fmt::color getLogLvlColor(bvd::LogLvl lvl) {
    switch (lvl)
    {
    case LogLvl::dbg: return fmt::color::gray;
    case LogLvl::inf: return fmt::color::light_gray;
    case LogLvl::wrn: return fmt::color::orange;
    case LogLvl::err: return fmt::color::red;
    case LogLvl::vip: return fmt::color::green;
    case LogLvl::ftl: return fmt::color::red;
    }
    return {};
}

constexpr std::string_view getLogModName(bvd::LogMod mod) {
    switch (mod) {
    case bvd::LogMod::gen:     return "gen   ";
    case bvd::LogMod::wcm:     return "wcm    ";
    }

    return "unknown";
}

struct Exception {
    std::string_view message() const {
        return { buffer.data(), buffer.size() };
    }

    fmt::memory_buffer& buffer;
};

#define WIDE_INT(x) L##x
#define WIDE_EXT(x) WIDE_INT(x)

#define LOGFNAME bvd::baseFilename(__FILE__)
#define WLOGFNAME bvd::baseFilename(WIDE_EXT(__FILE__))

#define BVD_LOG(MOD, LVL, FORMAT, ...)                                                                       \
    if (bvd::logLevels[std::to_underlying(MOD)] <= LVL) {                                                    \
        fmt::memory_buffer loggerBuffer;                                                                     \
        fmt::format_to(std::back_inserter(loggerBuffer), "{} {} {:<30}[{:<4}]: " FORMAT "\n",                \
            bvd::getLogLvlName(LVL), bvd::getLogModName(MOD), LOGFNAME, __LINE__ __VA_OPT__(,) __VA_ARGS__); \
        std::string_view str(loggerBuffer.data(), loggerBuffer.size());                                      \
        fmt::print(fg(bvd::getLogLvlColor(LVL)), "{}", str);                                                 \
    }

#define BVD_THROW(FORMAT, ...)                                                                                         \
    {                                                                                                                  \
        fmt::memory_buffer exceptionBuffer;                                                                            \
        bvd::Exception     bvdException{exceptionBuffer};                                                              \
        fmt::format_to(std::back_inserter(exceptionBuffer), FORMAT __VA_OPT__(, ) __VA_ARGS__);                        \
        BVD_LOG(bvd::LogMod::gen, bvd::LogLvl::err, "{}", bvdException.message());                                     \
        throw bvdException;                                                                                            \
    }


} // namespace bvd
