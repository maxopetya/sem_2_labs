//
// Created by maxop on 15/04/2026.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <iterator>
#include <string>



// Класс Книга
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

    // Оператор сравнения для сортировки по ФИО автора
    bool operator<(const Book& other) const {
        return author_ < other.author_;
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
            // Парсинг строки с разделителем ';'
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

int main() {
    // Чтение данных из файла input.txt
    std::ifstream fin("input.txt");
    if (!fin.is_open()) {
        std::cerr << "Cannot open input.txt" << std::endl;
        return 1;
    }

    // Заполнение исходного контейнера vector
    std::vector<Book> vec;
    Book book;
    while (fin >> book) {
        vec.push_back(book);
    }
    fin.close();

    // Открытие файла для записи результатов
    std::ofstream fout("output.txt");
    if (!fout.is_open()) {
        std::cerr << "Cannot open output.txt" << std::endl;
        return 1;
    }

    // Вывод исходного контейнера
    fout << "Original container (vector):" << std::endl;
    std::cout << "Original container (vector):" << std::endl;
    for (const auto& b : vec) {
        fout << b << std::endl;
        std::cout << b << std::endl;
    }
    fout << std::endl;
    std::cout << std::endl;

    // Сортировка по ФИО автора (используется operator<)
    std::sort(vec.begin(), vec.end());

    // Вывод отсортированного контейнера
    fout << "Sorted by author (vector):" << std::endl;
    std::cout << "Sorted by author (vector):" << std::endl;
    for (const auto& b : vec) {
        fout << b << std::endl;
        std::cout << b << std::endl;
    }
    fout << std::endl;
    std::cout << std::endl;

    // Копирование из vector в deque с помощью std::copy
    std::deque<Book> deq(vec.size());
    std::copy(vec.begin(), vec.end(), deq.begin());

    // Вывод скопированного контейнера
    fout << "Copied container (deque):" << std::endl;
    std::cout << "Copied container (deque):" << std::endl;
    for (const auto& b : deq) {
        fout << b << std::endl;
        std::cout << b << std::endl;
    }

    fout.close();
    std::cout << std::endl << "Results written to output.txt" << std::endl;

    return 0;
}
