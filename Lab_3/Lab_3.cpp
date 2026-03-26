#include <iostream>
#include <unordered_set>
#include <stdexcept>
#include <unordered_map>
#include <string>

template<class T>
class MyUnique {
private:
    T* p = nullptr;
    static inline std::unordered_set<T*> v;

public:
    //констурктор
    explicit MyUnique(T* ptr) {
        if (ptr == nullptr) {
            p = nullptr;
            return;
        }
        if (v.count(ptr) == 0) {
            p = ptr;
            v.insert(ptr);
        } else {
            throw std::runtime_error("Not unique");
        }
    }

    //деструктор
    ~MyUnique() {
        if (p != nullptr) {
            v.erase(p);
            delete p;
        }
    }

    //запрет копирования
    MyUnique(const MyUnique&) = delete;
    MyUnique& operator=(const MyUnique&) = delete;

    MyUnique(MyUnique&& rhs) noexcept {
        p = rhs.p;
        rhs.p = nullptr;
    }

    //Оператор перемещающего присваивания
    MyUnique& operator=(MyUnique&& rhs) noexcept {
        if (this != &rhs) {
            if (p != nullptr) {
                v.erase(p);
                delete p;
            }
            p = rhs.p;
            rhs.p = nullptr;
        }
        return *this;
    }

    T* get() const { return p; }
    T& operator*() { return *p; }
    T* operator->() { return p; }
};

template<class T>
class MyShared {
private:
    T* p = nullptr;
    static inline std::unordered_map<T*, int> v;

    void release() {
        if (p != nullptr) {
            v[p]--;
            if (v[p] == 0) {
                v.erase(p);
                delete p;
            }
            p = nullptr;
        }
    }

public:

    //конструктор
    explicit MyShared(T* ptr) {
        p = ptr;
        if (p != nullptr) {
            v[p]++;
        }
    }

    //деструктор
    ~MyShared() {
        release();
    }

    //конструктор копирования
    MyShared(const MyShared& rhs) {
        p = rhs.p;
        if (p != nullptr) {
            v[p]++;
        }
    }

    //конструктор перемещения
    MyShared(MyShared&& rhs) noexcept {
        p = rhs.p;
        rhs.p = nullptr;
    }

    //Оператор копирующего присваивания
    MyShared& operator=(const MyShared& rhs) {
        if (this != &rhs) {
            release();
            p = rhs.p;
            if (p != nullptr) {
                v[p]++;
            }
        }
        return *this;
    }

    //Оператор перемещающего присваивания
    MyShared& operator=(MyShared&& rhs) noexcept {
        if (this != &rhs) {
            release();
            p = rhs.p;
            rhs.p = nullptr;
        }
        return *this;
    }

    T* get() const { return p; }
    T& operator*() { return *p; }
    T* operator->() { return p; }

    int count() const {
        if (p == nullptr) return 0;
        return v[p];
    }
};


struct Tracer {
    std::string name;
    Tracer(std::string n) : name(n) {
        std::cout << "  [+] Tracer created: " << name << std::endl;
    }
    ~Tracer() {
        std::cout << "  [-] Tracer destroyed: " << name << std::endl;
    }
    void act() {
        std::cout << "      Tracer " << name << " is active." << std::endl;
    }
};

int main() {
    std::cout << "TESTING MyUnique\n";
    {
        std::cout << "Creating u1\n";
        MyUnique<Tracer> u1(new Tracer("Unique_1"));
        u1->act();

        std::cout << "\nMoving u1 to u2\n";
        MyUnique<Tracer> u2 = std::move(u1);
        std::cout << "u1 pointer: " << (u1.get() == nullptr ? "nullptr" : "active") << std::endl;
        u2->act();

        std::cout << "\nAssigning new object to u2\n";
        u2 = MyUnique<Tracer>(new Tracer("Unique_2"));
    }
    std::cout << "End of MyUnique scope\n\n";

    std::cout << "TESTING MyShared\n";
    {
        std::cout << "Creating s1\n";
        MyShared<Tracer> s1(new Tracer("Shared_1"));
        std::cout << "s1 count: " << s1.count() << std::endl;

        {
            std::cout << "\nCopying s1 to s2\n";
            MyShared<Tracer> s2 = s1;
            std::cout << "Ref count: " << s1.count() << std::endl;

            std::cout << "\nMoving s2 to s3\n";
            MyShared<Tracer> s3 = std::move(s2);
            std::cout << "Ref count: " << s1.count() << std::endl;
            std::cout << "s2 pointer: " << (s2.get() == nullptr ? "nullptr" : "active") << std::endl;
        }

        std::cout << "\nAfter nested block, s1 count: " << s1.count() << "\n";
    }
    std::cout << "End of MyShared scope\n";

    return 0;
}