#pragma once

#include "book.hpp"
#include <cmath>

namespace bookdb::comp {

// компараторы для сортировки книг по разным полям, с разрешением конфликтов

// компаратор по прочитавшим
struct LessByReadCount {
    constexpr bool operator()(const Book &lh, const Book &rh) const { return lh.read_count < rh.read_count; }
};

// компаратор по рейтингу
struct LessByRating {
    constexpr bool operator()(const Book &lh, const Book &rh) const {
        if (lh.rating == rh.rating) {
            return LessByReadCount{}(lh, rh);  // далее - по прочитавшим
        }
        return std::isless(lh.rating, rh.rating);
    }
};

// компаратор по году
struct LessByYear {
    constexpr bool operator()(const Book &lh, const Book &rh) const {
        if (lh.year == rh.year) {
            return LessByRating{}(lh, rh);  // далее - по рейтингу
        }
        return lh.year < rh.year;
    }
};

// компаратор по названию
struct LessByTitle {
    constexpr bool operator()(const Book &lh, const Book &rh) const {
        if (lh.title == rh.title) {
            return LessByYear{}(lh, rh);  // далее - по году
        }
        return lh.title < rh.title;
    }
};

// компаратор по автору
struct LessByAuthor {
    constexpr bool operator()(const Book &lh, const Book &rh) const {
        if (lh.author == rh.author) {
            return LessByTitle{}(lh, rh);  // далее - по названию
        }
        return lh.author < rh.author;
    }
};

}  // namespace bookdb::comp