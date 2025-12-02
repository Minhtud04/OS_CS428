#include "fixed_hashmap.h"
FixedHashMap::FixedHashMap(int size) {
    if (size <= 0) {
        size = MAX_SIZE;
    }
    num_buckets_ = size;
    // Allocate the array of buckets
    buckets_ = new Bucket[size];
}

FixedHashMap::~FixedHashMap() {
    for (int i = 0; i < num_buckets_; ++i) {
        Node* current = buckets_[i].head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] buckets_;
}


bool FixedHashMap::InsertHelper(int key, int value) {
    int index = GetIndex(key);
    Node* current = buckets_[index].head;
    
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return false; 
        }
        current = current->next;
    }
    Node* newNode = new Node(key, value);
    newNode->next = buckets_[index].head;
    buckets_[index].head = newNode;
    return true;
}

bool FixedHashMap::RemoveHelper(int key) {
    int index = GetIndex(key);
    Node* current = buckets_[index].head;
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            // Found it, unlink it
            if (prev == nullptr) {
                buckets_[index].head = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false; 
}

bool FixedHashMap::SearchHelper(int key, int& outValue) {
    int index = GetIndex(key);
    Node* current = buckets_[index].head;
    
    while (current != nullptr) {
        if (current->key == key) {
            outValue = current->value;
            return true;
        }
        current = current->next;
    }
    return false; 
}



bool FixedHashMap::InsertGlobalLock(int key, int value) {
    std::lock_guard<std::mutex> lock(global_lock_);
    return InsertHelper(key, value);
}

bool FixedHashMap::RemoveGlobalLock(int key) {
    std::lock_guard<std::mutex> lock(global_lock_);
    return RemoveHelper(key);
}

bool FixedHashMap::SearchGlobalLock(int key, int& outValue) {
    std::lock_guard<std::mutex> lock(global_lock_);
    return SearchHelper(key, outValue);
}



bool FixedHashMap::InsertAdvancedLock(int key, int value) {
    int index = GetIndex(key);
    // Lock ONLY the specific bucket
    std::lock_guard<std::mutex> lock(buckets_[index].lock);
    return InsertHelper(key, value);
}

bool FixedHashMap::RemoveAdvancedLock(int key) {
    int index = GetIndex(key);
    std::lock_guard<std::mutex> lock(buckets_[index].lock);
    return RemoveHelper(key);
}

bool FixedHashMap::SearchAdvancedLock(int key, int& outValue) {
    int index = GetIndex(key);
    std::lock_guard<std::mutex> lock(buckets_[index].lock);
    return SearchHelper(key, outValue);
}