#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {
template <typename T, typename... Args>
concept BookContainerLike = requires(T cont, Args &&...args) {
    typename T::iterator;
    typename T::const_iterator;
    requires std::same_as<typename T::value_type, Book>;
    { cont.begin() } -> std::same_as<typename T::iterator>;
    { cont.cbegin() } -> std::same_as<typename T::const_iterator>;
    { cont.end() } -> std::same_as<typename T::iterator>;
    { cont.cend() } -> std::same_as<typename T::const_iterator>;
    { cont.size() } -> std::convertible_to<std::size_t>;
    { cont.emplace_back(std::forward<Args>(args)...) } -> std::same_as<Book &>;
};

template <typename T>
concept BookIterator = std::input_iterator<T> && std::same_as<std::iter_value_t<T>, Book> &&
                       std::convertible_to<std::iter_reference_t<T>, const Book &>;

template <typename S, typename I>
concept BookSentinel = BookIterator<I> && std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = requires(P pred, Book &b) {
    { pred(b) } -> std::same_as<bool>;
};

template <typename C>
concept BookComparator = requires(C cmp, Book &lh, Book &rh) {
    { cmp(lh, rh) } -> std::same_as<bool>;
};

}  // namespace bookdb