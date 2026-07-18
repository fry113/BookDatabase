#pragma once

#include <book.hpp>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;

    bool operator()(std::string_view sv, const Book &b) const noexcept { return sv < b.author; }
    bool operator()(const Book &b, std::string_view sv) const noexcept { return b.author < sv; }
    bool operator()(std::string_view lh, std::string_view rh) const noexcept { return lh < rh; }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(std::string_view sv, const Book &b) const noexcept { return sv == b.author; }
    bool operator()(const Book &b, std::string_view sv) const noexcept { return sv == b.author; }
    bool operator()(std::string_view lh, std::string_view rh) const noexcept { return lh == rh; }
};

struct TransparentStringHash {
    using is_transparent = void;
    std::size_t operator()(const Book &b) const noexcept { return std::hash<std::string_view>{}(b.author); }
    std::size_t operator()(const std::string_view &sv) const noexcept { return std::hash<std::string_view>{}(sv); }
};

}  // namespace bookdb
