#ifndef FIXED_HASHMAP_H
#define FIXED_HASHMAP_H

#include <mutex>
#include <vector>

#define MAX_SIZE 10007

struct Node {
    int key;
    int value;
    Node* next;
    Node(int k, int v) : key(k), value(v), next(nullptr) {}
};

struct Bucket {
    Node* head;
    std::mutex lock;

    Bucket() : head(nullptr) {}
};

class FixedHashMap {
public:
    explicit FixedHashMap(int size = MAX_SIZE);
    ~FixedHashMap();

    bool InsertGlobalLock(int key, int value);
    bool RemoveGlobalLock(int key);
    bool SearchGlobalLock(int key, int& outValue);

    bool InsertAdvancedLock(int key, int value);
    bool RemoveAdvancedLock(int key);
    bool SearchAdvancedLock(int key, int& outValue);

private:
    int num_buckets_;
    Bucket* buckets_; 
    std::mutex global_lock_;

    bool InsertHelper(int key, int value);
    bool RemoveHelper(int key);
    bool SearchHelper(int key, int& outValue);

    int GetIndex(int key) const {
        return key % num_buckets_; 
    }
};

#endif // FIXED_HASHMAP_H