#include "RBTree.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <cassert>

#include <mutex>
#include <condition_variable>


using namespace std;

void test_basic_insert_search() {
    cout << "=== Test: Basic Insert & Search ===" << endl;
    RBTree tree;
    
    // Insert some values
    tree.InsertGlobalLock(10, 100);
    tree.PrintTreePretty();

    tree.InsertGlobalLock(20, 200);
    tree.PrintTreePretty();

    tree.InsertGlobalLock(5, 50);
    tree.PrintTreePretty();

    tree.InsertGlobalLock(15, 150);
    tree.PrintTreePretty();


    // Search for them
    assert(tree.SearchGlobalLock(10) == 100);
    assert(tree.SearchGlobalLock(20) == 200);
    assert(tree.SearchGlobalLock(5) == 50);
    assert(tree.SearchGlobalLock(15) == 150);
    assert(tree.SearchGlobalLock(999) == -1);  // Not found
    
    cout << "PASSED!" << endl << endl;

    // assert(tree.CheckTreeBalance() == true);
}

void test_basic_rotation() {
    RBTree tree;
    int n = 20;
    for (int i = n; i >= 0; i--) {
        std::cout << i << std::endl;
        tree.InsertGlobalLock(i, i * 10);
        tree.PrintTreePretty();

        assert(tree.CheckTreeBalance() == true);
    }

    tree.PrintTreePretty();
    cout << "PASSED!" << endl << endl;



}

void test_basic_concurrency() {
    cout << "=== Test: Basic Concurrency (Multi-threaded Insert & Search) ===" << endl;
    RBTree tree;
    
    const int NUM_THREADS = 8;
    const int OPS_PER_THREAD = 1000;
    vector<thread> threads;
    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&tree, t, OPS_PER_THREAD]() {
            int start_key = t * OPS_PER_THREAD;
            int end_key = start_key + OPS_PER_THREAD;

            for (int k = start_key; k < end_key; ++k) {
                bool success = tree.InsertGlobalLock(k, k * 10);
                if (success) {
                    int val = tree.SearchGlobalLock(k);
                    if (val != k * 10) {
                        cerr << "Error: Thread " << t << " inserted " << k 
                             << " but read back " << val << endl;
                        exit(1);
                    }
                }
                // if (k % 50 == 0) {
                //     tree.SearchGlobalLock(rand() % OPS_PER_THREAD); 
                // }
            }
        });
    }
    for (auto& th : threads) {
        th.join();
    }

    assert(tree.CheckTreeBalance() == true);
    int total_keys = NUM_THREADS * OPS_PER_THREAD;
    for (int i = 0; i < total_keys; ++i) {
        if (tree.SearchGlobalLock(i) == -1) {
            cerr << "Missing key: " << i << endl;
            assert(false);
        }
    }
    cout << "PASSED!" << endl;
}

void test_massive_concurrency() {
    cout << "=== Test: Massive Concurrency  ===" << endl;
    const int N = 1000;
    RBTree tree;
    
    // --- PHASE 1: MASSIVE INSERTION ---
    std::vector<std::thread> writers;
    writers.reserve(N);
    for (int i = 0; i < N; ++i) {
        writers.emplace_back([&tree, i]() {
            tree.InsertGlobalLock(i, i * 10);
        });
    }
    for (auto& t : writers) {
        if (t.joinable()) t.join();
    }

    // Read
    std::vector<std::thread> readers;
    readers.reserve(N);

    for (int i = 0; i < N; ++i) {
        readers.emplace_back([&tree, i]() {
            int val = tree.SearchGlobalLock(i);
            if (val != i * 10) {
                std::cerr << "Error: Thread " << i << " expected " << i * 10 
                          << " but got " << val << std::endl;
                std::exit(1); // Fail fast
            }
        });
    }
    for (auto& t : readers) {
        if (t.joinable()) t.join();
    }
    
    std::cout << "PASSED!" << std::endl;
}


int main() {
    test_basic_insert_search();
    test_basic_rotation();
    test_basic_concurrency();
    test_massive_concurrency();
}