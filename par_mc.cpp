#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <chrono>
#ifdef _OPENMP
#include <omp.h>
#endif

// 0 = serial, 1 = std::thread, 2 = OpenMP
const int MODE = 2;          // change this number to switch
const int NUM_THREADS = 16;   // used for threaded and OpenMP versions
const long N = 100'000'000;  // number of samples


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

//montecarlo_threaded (each thread does its own count)
double montecarlo_threaded(long N, int num_threads) {
    std::vector<std::thread> threads;
    std::vector<long> counts(num_threads, 0);

    long base = N / num_threads;
    long rem  = N % num_threads;

    for (int t = 0; t < num_threads; ++t) {
        long n_samples = base + (t < rem ? 1 : 0);  // distribute remainder

        threads.emplace_back([n_samples, t, &counts]() {
            std::mt19937 gen(42 + t);  // different seed per thread
            std::uniform_real_distribution<> dist(0.0, 1.0);

            long local_count = 0;
            for (long i = 0; i < n_samples; ++i) {
                double x = dist(gen), y = dist(gen);
                if (x*x + y*y <= 1.0) ++local_count;
            }

            counts[t] = local_count;  // store result for this thread
        });
    }

    for (auto &th : threads) {
        th.join();
    }

    long total_count = 0;
    for (int t = 0; t < num_threads; ++t) {
        total_count += counts[t];
    }

    return 4.0 * static_cast<double>(total_count) / static_cast<double>(N);
}


// montecarlo_openmp (use reduction(+:count))
double montecarlo_openmp(long N, int num_threads) {
#ifdef _OPENMP
    omp_set_num_threads(num_threads);
#endif

    long total_count = 0;

    #pragma omp parallel
    {
#ifdef _OPENMP
        int tid = omp_get_thread_num();
#else
        int tid = 0;
#endif
        std::mt19937 gen(42 + tid);  // different seed per thread
        std::uniform_real_distribution<> dist(0.0, 1.0);

        long local_count = 0;

        #pragma omp for
        for (long i = 0; i < N; ++i) {
            double x = dist(gen), y = dist(gen);
            if (x*x + y*y <= 1.0) ++local_count;
        }

        #pragma omp atomic
        total_count += local_count;
    }

    return 4.0 * static_cast<double>(total_count) / static_cast<double>(N);
}


int main() {
    double pi_sum = 0.0;
    double time_sum = 0.0;

    for (int r = 1; r <= 3; r++) {
        auto t1 = std::chrono::high_resolution_clock::now();

        double pi;
        if (MODE == 0) {
            pi = montecarlo_serial(N);
        } else if (MODE == 1) {
            pi = montecarlo_threaded(N, NUM_THREADS);
        } else {
            pi = montecarlo_openmp(N, NUM_THREADS);
        }

        auto t2 = std::chrono::high_resolution_clock::now();
        double time_sec = std::chrono::duration<double>(t2 - t1).count();

        // accumulate for averaging
        pi_sum += pi;
        time_sum += time_sec;

        // print each run result
        std::cout << "Run " << r
                  << " | MODE=" << MODE
                  << " | N=" << N
                  << " | THREADS=" << NUM_THREADS
                  << " | pi=" << pi
                  << " | time=" << time_sec << " s"
                  << std::endl;
    }

    // Print averaged results at the end
    std::cout << "Average pi = " << (pi_sum / 3.0)
              << "  | Average time = " << (time_sum / 3.0) << " s"
              << std::endl;

    return 0;
}
