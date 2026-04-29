#pragma once

#include <algorithm>
#include <array>
#include <format>
#include <stdexcept>
#include <string_view>
#include <type_traits>

namespace bookdb {

// Жанры хранятся в enum и в string_view для бустрой конвертации
enum class Genre : size_t { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown, GenreQuan_ };
constexpr std::array<std::string_view, static_cast<size_t>(Genre::GenreQuan_)> GenreStrings{
    "Fiction", "NonFiction", "SciFi", "Biography", "Mystery", "Unknown"};

// можно было и в цикле поискать, а поток через .at(), но решил применить std::distance()
constexpr Genre GenreFromString(std::string_view s) {
    auto it = std::find(GenreStrings.begin(), GenreStrings.end(), s);
    return (it == GenreStrings.end()) ? Genre::Unknown : static_cast<Genre>(std::distance(GenreStrings.begin(), it));
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year{0};
    Genre genre{Genre::Unknown};
    double rating{.0};
    int read_count{0};

    constexpr auto get_title() const noexcept { return title.c_str(); }

    // конструктор через bookdb::Genre
    template <typename T>
        requires std::is_same_v<T, Genre>
    constexpr Book(std::string_view a_title, std::string_view a_author, int a_year, T a_genre, double a_rating,
                   int a_read_count) noexcept
        : title(a_title), author(a_author), year(a_year), genre(a_genre), rating(a_rating), read_count(a_read_count) {}

    // конструктор через std::string_view
    template <typename T>
        requires std::is_same_v<T, std::string_view>
    constexpr Book(std::string_view a_title, std::string_view a_author, int a_year, T a_genre, double a_rating,
                   int a_read_count) noexcept
        : title(a_title), author(a_author), year(a_year), genre(GenreFromString(a_genre)), rating(a_rating),
          read_count(a_read_count) {}

    // конструктор по умолчанию
    constexpr Book() noexcept = default;

    // конструкторы копирования и перемещения по умолчанию
    constexpr Book(const Book &) noexcept = default;
    constexpr Book(Book &&) noexcept = default;

    // операторы копирования и перемещения по умолчанию
    Book &operator=(const Book &) = default;
    Book &operator=(Book &&) noexcept = default;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// для std::formatter<Book>
template <>
struct formatter<bookdb::Book, char> {
    template <typename FormatContext>
    auto format(const bookdb::Book &b, FormatContext &fc) const {
        return format_to(fc.out(), "\"{}\" written by {} in {}. Genre: {}. {} people read and rated: {}", b.title,
                         b.author, b.year, b.genre, b.read_count, b.rating);
    }

    constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }
};

}  // namespace std
