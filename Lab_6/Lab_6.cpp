//
// Created by maxop on 15/04/2026.
//

#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

// Класс исключения, производный от invalid_argument
class PrimeException : public std::invalid_argument {
private:
    int value_; // Значение недопустимого объекта
public:
    // Конструктор исключения
    PrimeException(const std::string& msg, int val)
        : std::invalid_argument(msg), value_(val) {}

    // Геттер значения
    int getValue() const { return value_; }

    // Метод печати всех данных об исключении
    void print() const {
        std::cout << "Exception: " << what() << std::endl;
        std::cout << "Invalid value: " << value_ << std::endl;
    }
};

// Класс "Простое число"
class PrimeNumber {
private:
    int value_; // Значение простого числа

    // Статический метод проверки числа на простоту
    static bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i <= static_cast<int>(std::sqrt(n)); ++i) {
            if (n % i == 0) return false;
        }
        return true;
    }

public:
    // Конструктор с проверкой и выбросом исключения
    PrimeNumber(int val) : value_(val) {
        if (!isPrime(val)) {
            throw PrimeException("Number is not prime", val);
        }
    }

    // Геттер значения
    int getValue() const { return value_; }

    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const PrimeNumber& p) {
        os << p.value_;
        return os;
    }
};

int main() {
    // Демонстрация создания допустимых объектов
    std::cout << "Creating PrimeNumber with value 7" << std::endl;
    try {
        PrimeNumber p1(7);
        std::cout << "PrimeNumber created: " << p1 << std::endl;
    } catch (const PrimeException& e) {
        e.print();
    }

    std::cout << std::endl;

    std::cout << "Creating PrimeNumber with value 13" << std::endl;
    try {
        PrimeNumber p2(13);
        std::cout << "PrimeNumber created: " << p2 << std::endl;
    } catch (const PrimeException& e) {
        e.print();
    }

    std::cout << std::endl;

    // Демонстрация исключения — число не является простым
    std::cout << "Creating PrimeNumber with value 10" << std::endl;
    try {
        PrimeNumber p3(10);
        std::cout << "PrimeNumber created: " << p3 << std::endl;
    } catch (const PrimeException& e) {
        e.print();
    }

    std::cout << std::endl;

    // Демонстрация исключения — число меньше 2
    std::cout << "Creating PrimeNumber with value 1" << std::endl;
    try {
        PrimeNumber p4(1);
        std::cout << "PrimeNumber created: " << p4 << std::endl;
    } catch (const PrimeException& e) {
        e.print();
    }

    std::cout << std::endl;

    // Демонстрация исключения — отрицательное число
    std::cout << "Creating PrimeNumber with value -5" << std::endl;
    try {
        PrimeNumber p5(-5);
        std::cout << "PrimeNumber created: " << p5 << std::endl;
    } catch (const PrimeException& e) {
        e.print();
    }

    return 0;
}
