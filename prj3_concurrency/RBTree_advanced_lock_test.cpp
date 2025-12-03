#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <random>
#include "RBTree.h" // Assumed header name based on your provided content

using namespace std;

void test_basic_insert_search() {
    cout << "=== Test: Basic Insert & Search (Advanced Lock) ===" << endl;
    RBTree tree;
    
    // Insert some values using Advanced Lock
    tree.InsertAdvancedLock(10, 100);
    tree.PrintTreePretty();

    tree.InsertAdvancedLock(20, 200);
    tree.PrintTreePretty();

    tree.InsertAdvancedLock(5, 50);
    tree.PrintTreePretty();

    tree.InsertAdvancedLock(15, 150);
    tree.PrintTreePretty();


    // Search for them using Advanced Lock
    assert(tree.SearchAdvancedLock(10) == 100);
    assert(tree.SearchAdvancedLock(20) == 200);
    assert(tree.SearchAdvancedLock(5) == 50);
    assert(tree.SearchAdvancedLock(15) == 150);
    assert(tree.SearchAdvancedLock(999) == -1);  // Not found
    
    cout << "PASSED!" << endl << endl;
}

void test_basic_rotation() {
    cout << "=== Test: Basic Rotation (Sequential Advanced Lock) ===" << endl;
    RBTree tree;
    int n = 20;
    // Insert in descending order to force rotations
    for (int i = n; i >= 0; i--) {
        // std::cout << "Inserting " << i << std::endl;
        tree.InsertAdvancedLock(i, i * 10);
        // tree.PrintTreePretty(); // Uncomment to debug structure

        assert(tree.CheckTreeBalance() == true);
    }

    tree.PrintTreePretty();
    cout << "PASSED!" << endl << endl;
}

void test_basic_concurrency() {
    cout << "=== Test: Basic Concurrency (Multi-threaded Insert & Search - Advanced Lock) ===" << endl;
    RBTree tree;
    
    const int NUM_THREADS = 8;
    const int OPS_PER_THREAD = 1000;
    vector<thread> threads;
    
    cout << "Launching " << NUM_THREADS << " threads..." << endl;

    for (int t = 0; t < NUM_THREADS; ++t) {
        threads.emplace_back([&tree, t, OPS_PER_THREAD]() {
            int start_key = t * OPS_PER_THREAD;
            int end_key = start_key + OPS_PER_THREAD;

            for (int k = start_key; k < end_key; ++k) {
                // 1. Insert with Fine-Grained Locking
                bool success = tree.InsertAdvancedLock(k, k * 10);
                
                // 2. Immediate Read-Back
                if (success) {
                    int val = tree.SearchAdvancedLock(k);
                    if (val != k * 10) {
                        cerr << "Error: Thread " << t << " inserted " << k 
                             << " but read back " << val << endl;
                        exit(1);
                    }
                }
            }
        });
    }
    
    for (auto& th : threads) {
        th.join();
    }

    cout << "All threads joined. Checking Balance..." << endl;
    assert(tree.CheckTreeBalance() == true);
    
    cout << "Verifying Data Integrity..." << endl;
    int total_keys = NUM_THREADS * OPS_PER_THREAD;
    for (int i = 0; i < total_keys; ++i) {
        if (tree.SearchAdvancedLock(i) == -1) {
            cerr << "Missing key: " << i << endl;
            assert(false);
        }
    }
    cout << "PASSED!" << endl << endl;
}

void test_massive_concurrency() {
    cout << "=== Test: Massive Concurrency (Advanced Lock) ===" << endl;
    const int N = 1000;
    RBTree tree;
    
    // --- PHASE 1: MASSIVE INSERTION ---
    std::cout << "[Phase 1] Spawning " << N << " writer threads..." << std::endl;
    std::vector<std::thread> writers;
    writers.reserve(N);
    for (int i = 0; i < N; ++i) {
        writers.emplace_back([&tree, i]() {
            tree.InsertAdvancedLock(i, i * 10);
        });
    }
    for (auto& t : writers) {
        if (t.joinable()) t.join();
    }
    std::cout << "[Phase 1] Writes done." << std::endl;

    // --- PHASE 2: MASSIVE SEARCH ---
    std::cout << "[Phase 2] Spawning " << N << " reader threads..." << std::endl;
    std::vector<std::thread> readers;
    readers.reserve(N);

    for (int i = 0; i < N; ++i) {
        readers.emplace_back([&tree, i]() {
            int val = tree.SearchAdvancedLock(i);
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
    return 0;
}