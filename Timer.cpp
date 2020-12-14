#pragma once
#include <chrono>
#include <iostream>

class Timer {
    std::chrono::high_resolution_clock::time_point start;
public:
    Timer() {
        start = std::chrono::high_resolution_clock::now();
    }

    ~Timer() {
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> d = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << std::fixed << d.count() << "s\n";
    }
};