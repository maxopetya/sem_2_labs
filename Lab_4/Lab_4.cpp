//
// Created by maxop on 30/03/2026.
//

#include <iostream>
#include <string>
using namespace std;

// Базовый класс - сотрудник предприятия
class Employee
{
protected:
    string name; // ФИО
    double salary; // оклад
    double bonusPercent; // надбавка за стаж (% от оклада за 1 год)
    int experience; // стаж в годах
public:
    // Конструктор для инициализации полей
    Employee(string n, double s, double bp, int e)
        : name(n), salary(s), bonusPercent(bp), experience(e) {}
    // Функция вычисления зарплаты (виртуальная для динамического полиморфизма)
    virtual double calcSalaryDynamic()
    {
        return salary + salary * bonusPercent / 100.0 * experience;
    }
    double calcSalaryStatic() {
        return salary + salary * bonusPercent / 100.0 * experience;
    }
    // Функция печати параметров (виртуальная)
    virtual void printDynamic()
    {
        cout << "Name: " << name << endl;
        cout << "Base salary: " << salary << endl;
        cout << "Bonus per year: " << bonusPercent << "%" << endl;
        cout << "Experience: " << experience << " years" << endl;
        cout << "Total salary: " << calcSalaryDynamic() << endl;
    }

    void printStatic()
    {
        cout << "Name: " << name << endl;
        cout << "Base salary: " << salary << endl;
        cout << "Bonus per year: " << bonusPercent << "%" << endl;
        cout << "Experience: " << experience << " years" << endl;
        cout << "Total salary: " << calcSalaryStatic() << endl;
    }
};

// Производный класс - начальник подразделения
class Manager : public Employee
{
    double managerBonus; // процентная надбавка за обязанности начальника
    string department; // название подразделения
public:
    // Конструктор с вызовом конструктора базового класса
    Manager(string n, double s, double bp, int e, double mb, string dep)
        : Employee(n, s, bp, e), managerBonus(mb), department(dep) {}

    // Переопределенная функция вычисления зарплаты
    double calcSalaryDynamic() override
    {
        // Вызов функции базового класса + надбавка за руководство
        return Employee::calcSalaryDynamic() + salary * managerBonus / 100.0;
    }

    // Переопределенная функция печати
    void printDynamic() override
    {
        Employee::printDynamic(); // Вызов функции базового класса
        cout << "Department: " << department << endl;
        cout << "Manager bonus: " << managerBonus << "%" << endl;
        cout << "Total salary (with manager bonus): " << calcSalaryDynamic() << endl;
    }
};

int main()
{
    // Создаем объект базового класса
    Employee emp("Ivanov Ivan", 50000, 5, 10);
    // Создаем объект производного класса
    Manager mgr("Petrov Petr", 60000, 5, 15, 20, "IT Department");

    cout << "=== Employee ===" << endl;
    emp.printDynamic();
    cout << endl;
    cout << "=== Manager ===" << endl;
    mgr.printDynamic();

    // Демонстрация динамического полиморфизма
    cout << endl << "=== Dynamic polymorphism ===" << endl;
    Employee *ptr; // Указатель типа базового класса
    ptr = &emp; // Указывает на объект базового класса
    cout << "Pointer to Employee:" << endl;
    ptr->printDynamic(); // Вызывается Employee::print
    cout << endl;
    ptr = &mgr; // Указывает на объект производного класса
    cout << "Pointer to Manager:" << endl;
    ptr->printDynamic();

    // Демонстрация статического полиморфизма
    cout << endl << "=== Static polymorphism (explicit class call) ===" << endl;
    cout << "Employee::print() called on Manager object:" << endl;
    mgr.Employee::printStatic();

    return 0;
}
