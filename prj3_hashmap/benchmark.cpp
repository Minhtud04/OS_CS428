#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>
#include "fixed_hashmap.h"

const int NUM_OPERATIONS = 500000; // Total ops per test run
const int KEY_RANGE = 10000;       // Range of keys (0 to 19999)
const int SIZE = 10007;
const int GET = 3;
const int ADD = 6;
const int DEL = 9;

int get_random_int(int min, int max) {
    static thread_local std::mt19937 generator(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void worker_task(FixedHashMap& map, int ops_per_thread, bool use_global) {
    int val = 0;
    for (int i = 0; i < ops_per_thread; ++i) {
        int key = get_random_int(0, KEY_RANGE); //random key from 1 - 20000
        int action = get_random_int(0, 9); // 0-9

        if (use_global) {
            if (action < GET) { 
                map.InsertGlobalLock(key, i);
            } else if (action < ADD) { 
                map.SearchGlobalLock(key, val);
            } else { 
                map.RemoveGlobalLock(key);
            }
        } else {
            if (action < GET) { 
                map.InsertAdvancedLock(key, i);
            } else if (action < ADD) { 
                map.SearchAdvancedLock(key, val);
            } else { 
                map.RemoveAdvancedLock(key);
            }
        }
    }
}

long long run_test(int num_threads, bool use_global) {
    FixedHashMap map(SIZE);
    std::vector<std::thread> threads;
    int ops_per_thread = NUM_OPERATIONS;

    auto start_time = std::chrono::high_resolution_clock::now();


    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_task, std::ref(map), ops_per_thread, use_global);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
}

int main() {
    std::vector<int> thread_counts = {1, 2, 4, 8, 16, 32, 64};

    std::cout << "Performance Benchmark (Time in ms)\n";
    std::cout << "Total Operations: (per thread) " << NUM_OPERATIONS << "\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << std::setw(10) << "Threads" 
              << std::setw(20) << "Global Lock (ms)" 
              << std::setw(20) << "Advanced Lock (ms)" << "\n";
    std::cout << "-----------------------------------------------------\n";

    for (int t : thread_counts) {
        long long global_time = run_test(t, true);
        long long advanced_time = run_test(t, false);

        std::cout << std::setw(10) << t 
                  << std::setw(20) << global_time 
                  << std::setw(20) << advanced_time << "\n";
    }

    return 0;
}