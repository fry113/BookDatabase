#pragma once

#include <set>
#include <string_view>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases
    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;
    using value_type = Book;
    using reference = Book &;
    using const_reference = const Book &;
    using size_type = typename BookContainer::size_type;
    using difference_type = typename BookContainer::difference_type;

    using AuthorContainer = std::set<std::string_view, TransparentStringLess>;

public:
    // Constructors and modifiers
    constexpr BookDatabase() = default;

    constexpr BookDatabase(std::initializer_list<Book> books) {
        for (auto &book : books) {
            EmplaceBack(std::move(book));
        }
    }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    iterator begin() { return books_.begin(); }
    const_iterator begin() const { return books_.begin(); }
    const_iterator cbegin() const { return books_.cbegin(); }
    iterator end() { return books_.end(); }
    const_iterator end() const { return books_.end(); }
    const_iterator cend() const { return books_.cend(); }

    size_type size() const { return books_.size(); }
    bool empty() const { return books_.empty(); }

    template <typename... Args>
    reference EmplaceBack(Args &&...args) {
        books_.emplace_back(std::forward<Args>(args)...);
        authors_.insert(books_.back().author);
        return books_.back();
    }

    reference EmplaceBack(Book &&book) {
        books_.emplace_back(std::move(book));
        authors_.insert(books_.back().author);
        return books_.back();
    }

    void PushBack(const Book &book) {
        books_.push_back(book);
        authors_.insert(books_.back().author);
    }

    // только const методы для доступа к данным
    const BookContainer &GetBooks() const { return books_; }
    const AuthorContainer &GetAuthors() const { return authors_; }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
