#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>
#include "RBTree.h"

using namespace std;

const int NUM_OPERATIONS = 100000; // Total ops per test run
const int KEY_RANGE = 100000;       // Range of keys (0 to 19999)

// Thresholds for operations (0-9)
// 50% Insert / 50% Search
const int INSERT_THRESHOLD = 5;    

int get_random_int(int min, int max) {
    static thread_local std::mt19937 generator(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void worker_task(RBTree& tree, int ops_per_thread, bool use_global) {
    volatile int val = 0; 
    for (int i = 0; i < ops_per_thread; ++i) {
        int key = get_random_int(0, KEY_RANGE);
        int action = get_random_int(0, 9); 

        if (use_global) {
            if (action < INSERT_THRESHOLD) { 
                tree.InsertGlobalLock(key, i);
            } else { 
                val = tree.SearchGlobalLock(key);
            }
        } else {
            if (action < INSERT_THRESHOLD) { 
                tree.InsertAdvancedLock(key, i);
            } else { 
                val = tree.SearchAdvancedLock(key);
            }
        }
    }
}

long long run_test(int num_threads, bool use_global) {

    RBTree tree; 
    
    std::vector<std::thread> threads;
    int ops_per_thread = NUM_OPERATIONS;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker_task, std::ref(tree), ops_per_thread, use_global);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
}

int main() {
    // Test configuration
    std::vector<int> thread_counts = {1, 2, 4, 8, 16, 32};

    std::cout << "RBTree Performance Benchmark (Time in ms)\n";
    std::cout << "Total Operations: (per thread) " << NUM_OPERATIONS << "\n";
    std::cout << "Ratio: 50% Insert / 50% Search\n";
    std::cout << "-----------------------------------------------------\n";
    std::cout << std::setw(10) << "Threads" 
              << std::setw(20) << "Global Lock (ms)" 
              << std::setw(20) << "Advanced Lock (ms)" << "\n";
    std::cout << "-----------------------------------------------------\n";

    for (int t : thread_counts) {
        // Run 1: Creates Tree A, runs Global Lock test, destroys Tree A
        long long global_time = run_test(t, true);
        
        // Run 2: Creates Tree B, runs Advanced Lock test, destroys Tree B
        long long advanced_time = run_test(t, false);

        std::cout << std::setw(10) << t 
                  << std::setw(20) << global_time 
                  << std::setw(20) << advanced_time << "\n";
    }

    return 0;
}