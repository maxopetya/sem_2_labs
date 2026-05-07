//
// Created by maxop on 08/05/2026.
// Часть 2, синхронизация через mutex + condition_variable
//

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

std::mutex coutMutex;        // мьютекс вывода
std::mutex notifyMutex;      // мьютекс оповещения
std::condition_variable cv;  // условная переменная
int finished = 0;            // счётчик завершившихся

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
        // Захват мьютекса
        coutMutex.lock();
        std::cout << name << " ";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::cout << a[i] << "\n";
        // Освобождение мьютекса
        coutMutex.unlock();
    }
    // Оповещение главного
    {
        std::lock_guard<std::mutex> lk(notifyMutex);
        finished++;
    }
    cv.notify_one();
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
    std::cout << "\n\n";

    // Первые два потока
    std::thread t1(threadFunc, "thread1", std::ref(arr), 0, N / 2);
    std::thread t2(threadFunc, "thread2", std::ref(arr), N / 2, N);

    // Главный ждёт оповещения
    for (int k = 1; k <= 2; ++k) {
        std::unique_lock<std::mutex> lk(notifyMutex);
        cv.wait(lk, [k] { return finished >= k; });
        // Захват для печати
        coutMutex.lock();
        std::cout << "Main: got notification " << k << "\n";
        coutMutex.unlock();
    }

    t1.join();
    t2.join();

    // Третий поток после
    std::thread t3(threadFunc, "thread3", std::ref(arr), 0, N);
    {
        std::unique_lock<std::mutex> lk(notifyMutex);
        cv.wait(lk, [] { return finished >= 3; });
    }
    coutMutex.lock();
    std::cout << "Main: got notification 3\n";
    coutMutex.unlock();
    t3.join();

    std::cout << "Main: all threads finished\n";
    return 0;
}
