#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <atomic>
#include <random>
#include "fixed_hashmap.h"

// ----------------------------------------------------------------
// 1. Simple Logic Test (Single Threaded)
// ----------------------------------------------------------------
void TestSimple() {
    std::cout << "[1/3] Running Simple Logic Test... ";
    FixedHashMap map(100);
    int val = 0;

    // Test Insert & Search
    map.InsertAdvancedLock(1, 100);
    assert(map.SearchAdvancedLock(1, val) == true);
    assert(val == 100);

    // Test Update
    map.InsertAdvancedLock(1, 200);
    assert(map.SearchAdvancedLock(1, val) == true);
    assert(val == 200);

    // Test Remove
    assert(map.RemoveAdvancedLock(1) == true);
    assert(map.SearchAdvancedLock(1, val) == false);
    assert(map.RemoveAdvancedLock(1) == false); // Remove non-existent

    std::cout << "PASSED" << std::endl;
}

// ----------------------------------------------------------------
// 2. Phased Concurrency Test (8 Threads: Add -> Read -> Delete)
// ----------------------------------------------------------------
void PhaseWorker(FixedHashMap& map, int thread_id, int items_per_thread) {
    int start_key = thread_id * items_per_thread;
    int end_key = start_key + items_per_thread;
    int val = 0;

    // Phase 1: Add
    for (int i = start_key; i < end_key; ++i) {
        map.InsertAdvancedLock(i, i * 2);
    }

    // Phase 2: Read (Verify)
    for (int i = start_key; i < end_key; ++i) {
        bool found = map.SearchAdvancedLock(i, val);
        assert(found == true);
        assert(val == i * 2);
    }

    // Phase 3: Delete
    for (int i = start_key; i < end_key; ++i) {
        bool removed = map.RemoveAdvancedLock(i);
        assert(removed == true);
    }
}

void TestConcurrencyPhased() {
    std::cout << "[2/3] Running Phased Concurrency Test (8 Threads)... ";
    FixedHashMap map(10007);
    std::vector<std::thread> threads;
    int num_threads = 8;
    int items_per_thread = 1000;

    // Since we can't easily barrier inside the worker without extra headers,
    // we will join threads between phases if strictly necessary, 
    // OR we can just run the whole sequence per thread on disjoint ranges.
    // The prompt asks for "8 thread concurrently add data in", then "8 thread read".
    // To strictly strictly follow that, we spawn for Add, Join, Spawn for Read, Join, etc.
    
    // 1. Concurrent Add
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&map, i, items_per_thread]() {
            int start = i * items_per_thread;
            for (int k = start; k < start + items_per_thread; ++k) map.InsertAdvancedLock(k, k);
        });
    }
    for (auto& t : threads) t.join();
    threads.clear();

    // 2. Concurrent Read
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&map, i, items_per_thread]() {
            int start = i * items_per_thread;
            int val;
            for (int k = start; k < start + items_per_thread; ++k) {
                if (!map.SearchAdvancedLock(k, val)) {
                    std::cerr << "Error: Key " << k << " missing!" << std::endl;
                    std::terminate();
                }
            }
        });
    }
    for (auto& t : threads) t.join();
    threads.clear();

    // 3. Concurrent Delete
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&map, i, items_per_thread]() {
            int start = i * items_per_thread;
            for (int k = start; k < start + items_per_thread; ++k) {
                if (!map.RemoveAdvancedLock(k)) {
                    std::cerr << "Error: Failed to remove " << k << std::endl;
                    std::terminate();
                }
            }
        });
    }
    for (auto& t : threads) t.join();

    // Final Verification: Map should be empty
    int dummy;
    for (int i = 0; i < num_threads * items_per_thread; ++i) {
        assert(map.SearchAdvancedLock(i, dummy) == false);
    }

    std::cout << "PASSED" << std::endl;
}

// ----------------------------------------------------------------
// 3. Complex Concurrency (Randomized High Contention)
// ----------------------------------------------------------------
// This stresses the linked list pointers by forcing many collisions.
void StressWorker(FixedHashMap& map, int ops_count) {
    std::mt19937 rng(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    std::uniform_int_distribution<int> key_dist(0, 100); // Small range = High collisions
    std::uniform_int_distribution<int> action_dist(0, 10);
    int val;

    for (int i = 0; i < ops_count; ++i) {
        int key = key_dist(rng);
        int action = action_dist(rng);

        if (action < 5) {
            map.InsertAdvancedLock(key, i);
        } else if (action < 8) {
            map.RemoveAdvancedLock(key);
        } else {
            map.SearchAdvancedLock(key, val);
        }
    }
}

void TestConcurrencyStress() {
    std::cout << "[3/3] Running Complex Stress Test (High Contention)... ";
    FixedHashMap map(100); // Small bucket count to force collisions
    std::vector<std::thread> threads;
    int num_threads = 16;
    
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(StressWorker, std::ref(map), 10000);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "PASSED (No crashes/deadlocks)" << std::endl;
}

int main() {
    TestSimple();
    TestConcurrencyPhased();
    TestConcurrencyStress();
    
    std::cout << "\nAll Tests Passed Successfully." << std::endl;
    return 0;
}