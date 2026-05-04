#include "book_database.hpp"
#include "statsistics.hpp"
#include <gtest/gtest.h>

using namespace bookdb;
using namespace std::string_view_literals;

void fillDB(BookDatabase<> &db) {
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
}

TEST(BookDatabase_Tests, EmptyDB) {
    BookDatabase db;
    EXPECT_TRUE(db.empty());
    EXPECT_EQ(db.size(), 0);
    EXPECT_EQ(db.GetAuthors().size(), 0);
    EXPECT_EQ(db.GetBooks().size(), 0);
    EXPECT_DOUBLE_EQ(calculateAverageRating(db), .0);
}

TEST(BookDatabase_Tests, EmplaceBack) {
    BookDatabase db;
    fillDB(db);
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 10);
}

TEST(BookDatabase_Tests, PushBack) {
    BookDatabase db;
    db.PushBack(Book("1984", "George Orwell", 1949, Genre::SciFi, 4., 190));
    db.PushBack(Book("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143));
    db.PushBack(Book("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120));
    db.PushBack(Book("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156));
    db.PushBack(Book("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178));
    db.PushBack(Book("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112));
    db.PushBack(Book("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98));
    db.PushBack(Book("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110));
    db.PushBack(Book("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203));
    db.PushBack(Book("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89));
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 10);
}

TEST(BookDatabase_Tests, InitializerListConstructor) {
    BookDatabase db{{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                    {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                    {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
                    {"To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156},
                    {"Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178},
                    {"The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112},
                    {"Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98},
                    {"Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110},
                    {"The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203},
                    {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89}};
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 10);
}

TEST(BookDatabase_Tests, BooksAndAuthorsQuantity) {
    BookDatabase db{{"1984", "George Orwell", 1949, Genre::SciFi, 4., 190},
                    {"Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143},
                    {"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120},
                    {"Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89}};
    EXPECT_FALSE(db.empty());
    EXPECT_EQ(db.size(), 4);
    EXPECT_EQ(db.GetAuthors().size(), 3);
    EXPECT_EQ(db.GetBooks().size(), 4);
}

TEST(BookDatabase_Tests, RatingCalculation) {
    BookDatabase db{};
    fillDB(db);

    auto avrRating = calculateAverageRating(db);

    EXPECT_DOUBLE_EQ(avrRating, 4.49);
}

TEST(Book_Tests, Constructors) {

    Book b1("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    EXPECT_EQ(b1.author, "George Orwell");
    EXPECT_EQ(b1.genre, Genre::SciFi);
    EXPECT_EQ(b1.title, "1984");
    EXPECT_EQ(b1.year, 1949);
    EXPECT_DOUBLE_EQ(b1.rating, 4.0);
    EXPECT_EQ(b1.read_count, 190);

    Book b2("1984", "George Orwell", 1949, "Fiction"sv, 4.0, 190);
    EXPECT_EQ(b2.author, "George Orwell");
    EXPECT_EQ(b2.genre, Genre::Fiction);
    EXPECT_EQ(b2.title, "1984");
    EXPECT_EQ(b2.year, 1949);
    EXPECT_DOUBLE_EQ(b2.rating, 4.0);
    EXPECT_EQ(b2.read_count, 190);
}

TEST(Book_Tests, GenreConversion) {

    EXPECT_EQ(GenreFromString("SciFi"sv), Genre::SciFi);
    EXPECT_EQ(GenreFromString("Fiction"sv), Genre::Fiction);
    EXPECT_EQ(GenreFromString("NonFiction"sv), Genre::NonFiction);
    EXPECT_EQ(GenreFromString("Mystery"sv), Genre::Mystery);
}

TEST(Book_Tests, WrongGenre) { EXPECT_EQ(GenreFromString("SMUT"sv), Genre::Unknown); }
