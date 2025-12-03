# Fixed sized hashmap

## Current features

### General Description
Fixed-size hash map with array and separate chaining. Implements both global lock and per-bucket (fine-grained) locking to analyze concurrency performance. Uses simple modulo hashing.

    - insert (key, val)

    - point_search (key)

**Advanced Locking strategy:**
All thread need to acquire a **shared_mutex**
1. Crabbing (hand - to hand) when search: acquire child lock, then release parent lock.
2. Insert: Crabbing (hand to hand), then try to insert. If required rotation, then release all locks (both node lock, and shared_mutex), acquire an **unique_lock<>** on this shared_mutex and re-do the insertion again.

This is a hybrid approach, as concurrency could happen if insertion requires no rotation. There is no deadlock here, since we only traverse in 1 way (top-down)


### Benchmark:
```
make benchmark
./benchmark
```

- Report Performance Benchmark (Time in ms) --- Total Operations: (per thread) 500000
```
-----------------------------------------------------
   Threads    Global Lock (ms)  Advanced Lock (ms)
-----------------------------------------------------
         1                  20                  32
         2                 164                 239
         4                 295                 308
         8                 596                 505
        16                1469                1082
        32                3459                1971
```
