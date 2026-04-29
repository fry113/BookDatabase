#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {

// лямбда для фильтрации по выремени публикации книги
inline auto YearBetween(int begin, int end) {
    return [begin, end](const Book &b) { return b.year >= begin && b.year <= end; };
}

// лямбда для фильтрации по рейтингу книги
inline auto RatingAbove(double rating) {
    return [rating](const Book &b) { return b.rating >= rating; };
}

// лямбда для фильтрации по жанру книги
inline auto GenreIs(Genre genre) {
    return [genre](const Book &b) { return b.genre == genre; };
}

// fold-expression для объединения нескольких предикатов в один по И
template <BookPredicate... Ps, typename T = Book>
auto all_of(Ps... preds) {
    return [preds...](const T &b) { return (preds(b) && ...); };
}

// fold-expression для объединения нескольких предикатов в один по ИЛИ
template <BookPredicate... Ps, typename T = Book>
auto any_of(Ps... preds) {
    return [preds...](const T &b) { return (preds(b) || ...); };
}

// сама фильтрующая функция
template <BookPredicate P, BookIterator It1, BookIterator It2>
auto filterBooks(It1 begin, It2 end, P pred)
    requires BookSentinel<It1, It2>
{
    std::vector<std::reference_wrapper<const Book>> ret;

    // резервируем память под все книги через std::distance(),
    // т.к. по условию задачи принимаем 2 итератора
    ret.reserve(std::distance(begin, end));

    // std::copy_if() + back_inserter() обеспечивают фильтрацию книг по предикату за O(n),
    // где n - количество книг в библиотеке
    std::copy_if(begin, end, std::back_inserter(ret), pred);

    // уменьшаем capacity до фактического размера, т.к. фильтрация может оставить меньше книг, чем было изначально
    ret.shrink_to_fit();

    return ret;
}

}  // namespace bookdb