# BookDatabase

BookDatabase - это учебный C++ проект хранилища книг.

Проект предоставляет контейнер для хранения книг, набор фильтров и компараторов, а также функции для вычисления статистики и выборок.

## Возможности

- Хранение коллекции книг в `BookDatabase`.
- Учет уникальных авторов внутри базы.
- Добавление книг через `EmplaceBack` и `PushBack`.
- Сортировка по автору, названию, году, рейтингу и числу прочтений.
- Фильтрация книг по предикатам (год, жанр, рейтинг, композиция предикатов).
- Статистика:
  - средний рейтинг по всей библиотеке;
  - средний рейтинг по жанрам;
  - гистограмма количества книг по авторам.
- Выборки:
  - случайная выборка книг;
  - top-N по выбранному компаратору.
- Unit-тесты на базе GoogleTest.

## Стек

- C++26
- CMake 3.30+
- Conan 2
- GoogleTest 1.13

## Структура проекта

- `include/` - заголовочные файлы библиотеки (`book.hpp`, `book_database.hpp`, фильтры, статистика, компараторы).
- `src/main.cpp` - пример использования BookDatabase.
- `tests/` - unit-тесты.
- `CMakeLists.txt` - конфигурация сборки.
- `conanfile.py` - зависимости и Conan-сборка.

## Сборка

### Вариант 1: через VS Code Task

В проекте уже есть задачи:

- `GCC: Build Debug`
- `GCC: Build Release`

Запустить можно через `Tasks: Run Task`.

### Вариант 2: через терминал

Из корня проекта:

```bash
mkdir -p build
cd build
conan build -b missing -s build_type=Debug ..
```

Для release-сборки:

```bash
mkdir -p build
cd build
conan build -b missing -s build_type=Release ..
```

После сборки бинарные файлы появятся в каталоге `build/`.

## Запуск

Запуск демонстрационного приложения:

```bash
./build/BookDB
```

## Тесты

Запуск unit-тестов:

```bash
./build/unit_tests
```

Или через CTest из каталога `build`:

```bash
cd build
ctest --output-on-failure
```

## Краткий пример использования

```cpp
#include "book_database.hpp"
#include "statsistics.hpp"

using namespace bookdb;

int main() {
    BookDatabase<> db;

    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4.0, 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);

    auto avg = calculateAverageRating(db);
    return avg > 0 ? 0 : 1;
}
```

## Примечания

- В проекте используется файл `include/statsistics.hpp` (название сохранено в текущем виде).
- Для корректной работы рекомендуется собирать проект в dev container или в окружении с установленными Conan и CMake.
