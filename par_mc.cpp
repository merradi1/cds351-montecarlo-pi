#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <chrono>
#ifdef _OPENMP
#include <omp.h>
#endif

double montecarlo_serial(long N) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dist(0.0, 1.0);
    long count = 0;
    for (long i = 0; i < N; ++i) {
        double x = dist(gen), y = dist(gen);
        if (x*x + y*y <= 1.0) ++count;
    }
    return 4.0 * count / N;
}

// TODO: montecarlo_threaded (each thread does its own count)
// TODO: montecarlo_openmp (use reduction(+:count))

int main() {
    const long N = 100'000'000;
    auto t1 = std::chrono::high_resolution_clock::now();
    double pi = montecarlo_serial(N);
    auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Estimated pi = " << pi
              << "  Time: " << std::chrono::duration<double>(t2 - t1).count() << " s\n";
}
