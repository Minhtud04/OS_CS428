# Fixed sized hashmap

## Current features

### General Description
Fixed-size hash map with array and separate chaining. Implements both global lock and per-bucket (fine-grained) locking to analyze concurrency performance. Uses simple modulo hashing.

    - insert (key, val)

    - delete (key)

    - point_search (key)


### Benchmark:
```
make benchmark
./benchmark
```

- Report Performance Benchmark (Time in ms) --- Total Operations: (per thread) 500000
```
   Threads    Global Lock (ms)  Advanced Lock (ms)
--------------------------------------------------
         1                  19                  18
         2                 150                  41
         4                 295                  74
         8                 602                  95
        16                1325                 168
        32                2781                 330
        64                5853                 578
```
The benchmark results clearly illustrate that the Global Lock implementation suffers from severe contention as concurrency increases, with execution time degrading from 19ms at 1 thread to over 5,800ms at 64 threads.

In contrast, the Advanced Lock (fine-grained) strategy significantly mitigates this bottleneck, maintaining superior throughput and finishing roughly 10 times faster at the highest load (578ms). 

The idea is simple. When using global lock, there is no concurrency at all, since each thread needs to wait for other to finish to start running. However, when use local lock, at most HASH_MAP_SIZE thread could run together, which significantly cut down the time.

## Citation
**Acknowledgments:**
1. I made this solely on my own. Prolly, I need to extend to an advanced hashmap version (extensible hashmap with bit extension, for example). However, this is a good project to understand about concurrency and global vs local lock.

2. For concurrency, I actually did a much more complex project on buffer pool manager and RAII locking (PageGuard) this semester - The CMU 16445 Intro to Database project 1. Therefore, I know a lot about concurrency and complex deadlock debug here.


**AI Assistance:**
This project was completed individually with the help of Gemini
My use of AI was focused on:
1. Create test & benchmark file
2. Consultation on how to create complex test cases and how to measure benchmark