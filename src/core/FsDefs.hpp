#pragma once

#include <string_view>

namespace bvd {

namespace detail {

template<size_t len>
constexpr char const* baseFilename(char const (&sz)[len], size_t n) {
    return (0 == n) ? sz : (sz[n] == '\\' || sz[n] == '/') ? sz + n + 1 : baseFilename(sz, --n);
}

template<size_t len>
constexpr wchar_t const* baseFilename(wchar_t const (&sz)[len], size_t n) {
    return (0 == n) ? sz : (sz[n] == '\\' || sz[n] == '/') ? sz + n + 1 : baseFilename(sz, --n);
}

} // namespace detail

template<size_t len>
constexpr char const* baseFilename(char const (&sz)[len]) {
    return detail::baseFilename(sz, len - 1);
}

template<size_t len>
constexpr wchar_t const* baseFilename(wchar_t const (&sz)[len]) {
    return detail::baseFilename(sz, len - 1);
}

constexpr std::string_view baseFilename(std::string_view filename)
{
    return filename.substr(filename.find_last_of('\\') + 1);
}

} // namespace bvd
