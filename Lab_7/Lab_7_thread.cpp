//
// Created by maxop on 08/05/2026.
// Часть 1, вариант через std::thread
//

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <string>
#include <chrono>

// Сортировка прямым выбором
void selectionSort(std::vector<double>& a, int from, int to) {
    for (int i = from; i < to - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < to; ++j) {
            if (a[j] < a[minIdx]) minIdx = j;
        }
        std::swap(a[i], a[minIdx]);
    }
}

// Потоковая функция
void threadFunc(const std::string& name, std::vector<double>& a, int from, int to) {
    selectionSort(a, from, to);
    for (int i = from; i < to; ++i) {
        std::cout << "\n" << name;
        // Небольшая задержка
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << " " << a[i];
    }
}

int main() {
    const int N = 10; // размер массива
    std::vector<double> arr(N);

    // Заполнение ГПСЧ
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 100.0);
    for (int i = 0; i < N; ++i) arr[i] = dist(gen);

    // Печать исходного
    std::cout << "Source array:";
    for (auto v : arr) std::cout << " " << v;
    std::cout << std::endl;

    // Первые два потока
    std::thread t1(threadFunc, "thread1", std::ref(arr), 0, N / 2);
    std::thread t2(threadFunc, "thread2", std::ref(arr), N / 2, N);

    // Ожидание завершения
    t1.join();
    t2.join();

    // Третий поток
    std::thread t3(threadFunc, "thread3", std::ref(arr), 0, N);
    t3.join();

    std::cout << "\nMain: all threads finished" << std::endl;
    return 0;
}
