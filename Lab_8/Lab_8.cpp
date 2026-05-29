//
// Created by maxop on 29/05/2026.
//

#include <iostream>
#include <fstream>
#include <set>
#include <unordered_set>
#include <string>
#include <functional>

// Класс Книга (тот же, что и в ЛР5)
class Book {
private:
    std::string author_;
    std::string title_;
    std::string publisher_;
    int year_;
    int pages_;

public:
    // Конструктор по умолчанию
    Book() : year_(0), pages_(0) {}

    // Конструктор с параметрами
    Book(const std::string& author, const std::string& title,
         const std::string& publisher, int year, int pages)
        : author_(author), title_(title), publisher_(publisher),
          year_(year), pages_(pages) {}

    // Геттеры
    std::string getAuthor() const { return author_; }
    std::string getTitle() const { return title_; }
    std::string getPublisher() const { return publisher_; }
    int getYear() const { return year_; }
    int getPages() const { return pages_; }

    // Оператор < нужен для set: упорядочивание по ФИО автора (как в ЛР4/ЛР5)
    bool operator<(const Book& other) const {
        return author_ < other.author_;
    }

    // Оператор == нужен для unordered_set: проверка уникальности при коллизиях
    bool operator==(const Book& other) const {
        return author_ == other.author_ &&
               title_ == other.title_ &&
               publisher_ == other.publisher_ &&
               year_ == other.year_ &&
               pages_ == other.pages_;
    }

    // Перегрузка оператора вывода в поток
    friend std::ostream& operator<<(std::ostream& os, const Book& b) {
        os << b.author_ << " | " << b.title_ << " | "
           << b.publisher_ << " | " << b.year_ << " | " << b.pages_ << " pages";
        return os;
    }

    // Перегрузка оператора ввода из потока
    friend std::istream& operator>>(std::istream& is, Book& b) {
        std::string line;
        if (std::getline(is, line)) {
            if (line.empty()) {
                is.setstate(std::ios::failbit);
                return is;
            }
            size_t p1 = line.find(';');
            size_t p2 = line.find(';', p1 + 1);
            size_t p3 = line.find(';', p2 + 1);
            size_t p4 = line.find(';', p3 + 1);
            if (p1 == std::string::npos || p2 == std::string::npos ||
                p3 == std::string::npos || p4 == std::string::npos) {
                is.setstate(std::ios::failbit);
                return is;
            }
            b.author_ = line.substr(0, p1);
            b.title_ = line.substr(p1 + 1, p2 - p1 - 1);
            b.publisher_ = line.substr(p2 + 1, p3 - p2 - 1);
            b.year_ = std::stoi(line.substr(p3 + 1, p4 - p3 - 1));
            b.pages_ = std::stoi(line.substr(p4 + 1));
        }
        return is;
    }
};

// Хеш-функтор для класса Book (требуется для unordered_set)
struct BookHash {
    std::size_t operator()(const Book& b) const {
        // Комбинируем хеши полей через XOR со сдвигом,
        // чтобы разные комбинации полей давали разные хеши
        std::size_t h1 = std::hash<std::string>{}(b.getAuthor());
        std::size_t h2 = std::hash<std::string>{}(b.getTitle());
        std::size_t h3 = std::hash<std::string>{}(b.getPublisher());
        std::size_t h4 = std::hash<int>{}(b.getYear());
        std::size_t h5 = std::hash<int>{}(b.getPages());
        return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
    }
};

int main() {
    // Открываем входной файл
    std::ifstream fin("input.txt");
    if (!fin.is_open()) {
        std::cerr << "Cannot open input.txt" << std::endl;
        return 1;
    }

    // Упорядоченный контейнер: автоматически сортирует по operator< (по автору)
    std::set<Book> bookSet;
    // Неупорядоченный контейнер: использует BookHash и operator==
    std::unordered_set<Book, BookHash> bookUnorderedSet;

    // Читаем книги из файла и сразу добавляем в оба контейнера
    Book book;
    while (fin >> book) {
        bookSet.insert(book);
        bookUnorderedSet.insert(book);
    }
    fin.close();

    // Вывод содержимого set (отсортирован по фамилии автора)
    std::cout << "=== set<Book> (sorted by author) ===" << std::endl;
    for (const auto& b : bookSet) {
        std::cout << b << std::endl;
    }
    std::cout << std::endl;

    // Вывод содержимого unordered_set (порядок зависит от хеш-таблицы)
    std::cout << "=== unordered_set<Book> (hash order) ===" << std::endl;
    for (const auto& b : bookUnorderedSet) {
        std::cout << b << std::endl;
    }
    std::cout << std::endl;

    // Дополнительная диагностика: размеры контейнеров
    std::cout << "set size: " << bookSet.size() << std::endl;
    std::cout << "unordered_set size: " << bookUnorderedSet.size() << std::endl;
    std::cout << "unordered_set bucket count: " << bookUnorderedSet.bucket_count() << std::endl;

    return 0;
}
