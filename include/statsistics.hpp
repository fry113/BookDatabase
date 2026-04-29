#pragma once

#include <algorithm>
#include <cstddef>
#include <flat_map>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <string_view>

#include "book_database.hpp"
#include "comparators.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

// гистограмма количества книг по авторам, с использованием flat_map
template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    /*
    не придумал более эффективного способа заполнить flat_map, чем:
    1. создать flat_map cont.GetAuthors()(O(n), где n - количество авторов, т.к. cont.authors_ уже отсортирован);
    2. пройтись по всем книгам и увеличить счетчик (O(m log n), где m - количество книг, n - количество авторов).
    в итоге получаем O(n + m log n).
    */
    // [1]
    std::flat_map<std::string_view, size_t, Comparator> ret(comp);
    std::transform(cont.GetAuthors().begin(), cont.GetAuthors().end(), std::inserter(ret, ret.end()),
                   [](std::string_view author) { return std::pair{author, size_t{0}}; });

    // [2]
    for (const auto &book : cont) {
        ++ret.at(book.author);
    }

    return ret;
}

// средний рейтинг книг по жанрам, с использованием одного или нескольких из доступных flat-контейнеров
template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &cont) {
    std::flat_map<Genre, double> ret;

    // array используется для накопления количества книг и суммы рейтингов по каждому жанру, п.что Genre - это enum,
    // который можно не хэшировать, а использовать как индекс в векторе, что быстрее flat_map
    std::array<std::pair<size_t, double>, static_cast<size_t>(Genre::GenreQuan_)> acc{};

    // проходим по всем книгам и накапливаем количество книг и сумму рейтингов по каждому жанру
    std::for_each(cont.begin(), cont.end(), [&acc](const Book &b) {
        ++(acc[static_cast<size_t>(b.genre)].first);
        acc[static_cast<size_t>(b.genre)].second += b.rating;
    });

    // вычисляем средний рейтинг для каждого жанра и заполняем flat_map
    // если книг в жанре нет, то средний рейтинг будет .0
    for (size_t g = 0; g < static_cast<size_t>(Genre::GenreQuan_); ++g) {
        auto val = (!acc[g].first) ? .0 : acc[g].second / acc[g].first;
        ret.emplace(static_cast<Genre>(g), val);
    }

    return ret;
}

// вычисления среднего рейтинга всех книг в библиотеке
template <BookContainerLike T>
auto calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.empty()) {
        return .0;
    }

    // здесь проще, просто суммируем рейтинги по всем книгам
    double ratingSum =
        std::transform_reduce(cont.begin(), cont.end(), .0, std::plus<>{}, [](const Book &b) { return b.rating; });

    // и делим на количество книг
    return ratingSum / cont.size();
}

// выборка по указанному количеству книг из библиотеки
template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t count) {
    // если запрошено больше книг, чем есть в библиотеке, то возвращаем все книги в случайном порядке
    count = std::min(count, cont.size());

    // не предполагается изменение списка книг, поэтому резервируется строго count элементов
    std::vector<std::reference_wrapper<const Book>> ret;
    ret.reserve(count);

    // std::sample() + mt19937 обеспечивают равномерную случайную выборку книг из библиотеки за O(n),
    // где n - количество книг в библиотеке
    std::sample(cont.begin(), cont.end(), std::back_inserter(ret), count, std::mt19937(std::random_device{}()));

    return ret;
}

// выборка из библиотеки указанного количества книг c наивысшим рейтингом
template <BookContainerLike T, typename Comparator = bookdb::comp::LessByRating>
auto getTopNBy(const BookDatabase<T> &cont, size_t count, Comparator comp = {}) {
    // если запрошено больше книг, чем есть в библиотеке, то возвращаем все книги
    count = std::min(count, cont.size());

    // ret резервируется под все вниги, т.к. будем вызывать для него std::nth_element и std::sort
    std::vector<std::reference_wrapper<const Book>> ret;
    ret.reserve(cont.size());

    // заполняем ret ссылками на книги через лямбду из библиотеки для дальнейшей обработки
    std::ranges::transform(cont, std::back_inserter(ret), [](const Book &book) { return std::cref(book); });

    // реверсируем компаратор LessByRating для сортировки по убыванию рейтинга, чтобы работать с обычными итераторами, а
    // не с reverse_iterator. для упрощения чтения и улучшения производительности
    auto GreaterByRating = [&comp](const auto &lh, const auto &rh) { return comp(rh.get(), lh.get()); };

    // nth_element отбирает top-N, sort упорядочивает только отобранную часть
    std::nth_element(ret.begin(), ret.begin() + count, ret.end(), GreaterByRating);
    std::sort(ret.begin(), ret.begin() + count, GreaterByRating);

    // удаляем все элементы после top-N
    ret.erase(ret.begin() + count, ret.end());

    return ret;
}

}  // namespace bookdb
