//
// Created by maxop on 12/03/2026.
//


#include <iostream>
#include <cstddef>
#include <stdexcept>

template <class T>
class vector {
public:
    // Конструктор по умолчанию
    vector()
    {
        this->vector_data = nullptr;
        this->vector_size = 0;
        this->vector_capacity = 0;
    }

    // Конструктор с параметрами
    vector(size_t n, const T& value)
    {
        this->vector_size = n;
        this->vector_capacity = n;
        this->vector_data = new T[this->vector_size];
        for (size_t i = 0; i < this->vector_size; i++) {
            this->vector_data[i] = value;
        }
    }

    // Конструктор копирования
    vector(const vector& vec)
    {
        this->vector_size = vec.vector_size;
        this->vector_capacity = vec.vector_capacity;
        this->vector_data = new T[this->vector_capacity];
        for (size_t i = 0; i < this->vector_size; i++)
        {
            this->vector_data[i] = vec.vector_data[i];
        }
    }

    // Деструктор
    ~vector()
    {
        delete[] this->vector_data;
    }

    // Оператор присваивания
    vector& operator= (const vector& vec)
    {
        // Проверка на самоприсваивание
        if (this == &vec) {
            return *this;
        }

        // Сохраняем старые данные на случай исключения
        T* new_data = nullptr;
        try {
            new_data = new T[vec.vector_capacity];
            for (size_t i = 0; i < vec.vector_size; i++)
            {
                new_data[i] = vec.vector_data[i];
            }
            delete[] this->vector_data;
            this->vector_data = new_data;
            this->vector_capacity = vec.vector_capacity;
            this->vector_size = vec.vector_size;
        }
        catch (...) {

            delete[] new_data;
            throw;
        }

        return *this;
    }

    // Неконстантный оператор доступа по индексу
    T& operator[] (size_t index)
    {
        if (index >= vector_size)
        {
            throw std::out_of_range("Index out of range");
        }
        return this->vector_data[index];
    }

    // Константный оператор доступа по индексу
    const T& operator[] (size_t index) const
    {
        if (index >= vector_size)
            throw std::out_of_range("Index out of range");
        return vector_data[index];
    }

    // Неконстантный метод at()
    T& at(size_t index)
    {
        if (index < this->vector_size)
        {
            return this->vector_data[index];
        }
        else
        {
            throw std::out_of_range("Index out of range");
        }
    }

    // Константный метод at()ц
    const T& at(size_t index) const
    {
        if (index < this->vector_size)
        {
            return this->vector_data[index];
        }
        else
        {
            throw std::out_of_range("Index out of range");
        }
    }

    // Возвращает ссылку на первый элемент
    T& front()
    {
        if (this->vector_size != 0)
        {
            return this->vector_data[0];
        }
        throw std::out_of_range("Vector is empty");
    }

    // Возвращает ссылку на последний элемент
    T& back()
    {
        if (this->vector_size != 0)
        {
            return this->vector_data[this->vector_size - 1];
        }
        throw std::out_of_range("Vector is empty");
    }

    // Константная версия front()
    const T& front() const
    {
        if (this->vector_size != 0)
        {
            return this->vector_data[0];
        }
        throw std::out_of_range("Vector is empty");
    }

    // Константная версия back()
    const T& back() const
    {
        if (this->vector_size != 0)
        {
            return this->vector_data[this->vector_size - 1];
        }
        throw std::out_of_range("Vector is empty");
    }

    // Возвращает указатель на внутренний массив
    T* data()
    {
        return this->vector_data;
    }

    // Константная версия data()
    const T* data() const
    {
        return this->vector_data;
    }

    // Возвращает итератор на начало
    T* begin()
    {
        return this->vector_data;
    }

    // Возвращает итератор на конец
    T* end()
    {
        return this->vector_data + this->vector_size;
    }

    // Константная версия begin()
    const T* begin() const
    {
        return this->vector_data;
    }

    // Константная версия end()
    const T* end() const
    {
        return this->vector_data + this->vector_size;
    }

    // Проверяет, пуст ли вектор
    bool empty() const
    {
        return this->vector_size == 0;
    }

    // Возвращает текущую емкость вектора
    size_t capacity() const
    {
        return this->vector_capacity;
    }

    // Возвращает текущий размер вектора
    size_t size() const
    {
        return this->vector_size;
    }

    // Резервирует память под new_capacity элементов
    void reserve(size_t new_capacity)
    {
        if (new_capacity <= this->vector_capacity)
        {
            return;
        }

        T* new_data = nullptr;
        try {
            new_data = new T[new_capacity];
            for (size_t i = 0; i < this->vector_size; i++)
            {
                new_data[i] = this->vector_data[i];
            }

            delete[] this->vector_data;
            this->vector_data = new_data;
            this->vector_capacity = new_capacity;
        }
        catch (...) {
            delete[] new_data;
            throw;
        }
    }

    // Очищает вектор
    void clear()
    {
        this->vector_size = 0;
    }

    // Вставляет элемент в указанную позицию
    void insert(T* pos, const T& value)
    {
        if (pos > this->vector_data + vector_size || pos < vector_data)
        {
            throw std::out_of_range("Incorrect position");
        }

        T* new_data = nullptr;
        try {
            new_data = new T[this->vector_capacity + 1];

            // Копируем элементы до позиции вставки
            for (size_t i = 0; i < (pos - vector_data); ++i)
            {
                new_data[i] = vector_data[i];
            }

            size_t index = pos - vector_data;
            new_data[index] = value; // Вставляем новый элемент

            // Копируем остальные элементы
            for (size_t i = index; i < this->vector_size; ++i)
            {
                new_data[i + 1] = vector_data[i];
            }

            delete[] this->vector_data;
            this->vector_data = new_data;
            ++this->vector_capacity;
            ++this->vector_size;
        }
        catch (...) {
            delete[] new_data;
            throw;
        }
    }

    // Добавляет элемент в конец вектора
    void push_back(const T& value)
    {
        insert(end(), value);
    }

    // Удаляет последний элемент
    void pop_back()
    {
        if (this->vector_size == 0) {
            throw std::out_of_range("Cannot pop_back from empty vector");
        }
        --this->vector_size;
    }

    // Обменивает содержимое двух векторов
    void swap(vector& other)
    {
        std::swap(this->vector_data, other.vector_data);
        std::swap(this->vector_size, other.vector_size);
        std::swap(this->vector_capacity, other.vector_capacity);
    }

private:
    T* vector_data;      // Указатель на массив элементов
    size_t vector_size;      // Текущее количество элементов
    size_t vector_capacity;  // Выделенная память (в элементах)
};