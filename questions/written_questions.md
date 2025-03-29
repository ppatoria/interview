High-Frequency Trading (HFT) firms and investment banks in India ask **advanced C++ interview coding questions** that focus on **low-latency systems, memory management, concurrency, and system design**. Below is a categorized list of **advanced C++ coding questions** commonly asked in such interviews:

---

## **1️⃣ Low-Latency & Performance Optimization**
🔹 **Cache Optimization & False Sharing**
- Implement a **lock-free** data structure optimized for **cache locality**.
- Write a **memory-efficient** LRU cache using **custom memory allocators**.

🔹 **SIMD & Compiler Optimizations**
- Optimize a **matrix multiplication algorithm** using **SIMD (AVX, SSE)**.
- Implement a **fast string comparison** using **intrinsics**.

🔹 **Branch Prediction & Pipelining**
- Given an array of **sorted and unsorted integers**, implement an **optimal searching strategy** that minimizes **branch mispredictions**.

---

## **2️⃣ Multi-Threading & Concurrency**
🔹 **Lock-Free Data Structures**
- Implement a **lock-free queue** using **CAS (Compare-And-Swap)**.
- Design a **wait-free stack**.

🔹 **Thread Synchronization & Deadlocks**
- Implement a **high-performance reader-writer lock**.
- Given multiple threads writing to a shared buffer, design an efficient **lock-free ring buffer**.

🔹 **False Sharing & NUMA Optimization**
- Optimize a multi-threaded **order matching engine** by **minimizing false sharing**.
- Implement a **NUMA-aware memory allocation** strategy.

---

## **3️⃣ Trading System Design**
🔹 **Order Matching Engine**
- Implement a **limit order book** with **O(log N) matching**.
- Design an order book that supports **GTT, Stop-Loss, and Iceberg orders**.

🔹 **Market Data Processing**
- Implement a **rate-limited FIX protocol parser**.
- Parse and aggregate **market depth data** for a high-speed trading system.

🔹 **Latency Measurement & Profiling**
- Implement a high-resolution **timestamping system** to measure function latencies at **nanosecond precision**.

---

## **4️⃣ Custom Memory Management**
🔹 **Custom Allocators**
- Implement a **pool allocator** that **avoids fragmentation**.
- Design an **arena allocator** for high-frequency memory allocations.

🔹 **Smart Pointers & RAII**
- Implement a **custom smart pointer** with **intrusive reference counting**.
- Optimize memory access for a **trading data cache**.

---

## **5️⃣ Advanced C++ Features**
🔹 **Templates & SFINAE**
- Implement a **type-trait utility** to detect if a function is **noexcept**.
- Use **concepts** to enforce constraints on a template-based order book.

🔹 **Compile-Time Computation**
- Implement a **constexpr hash function** for compile-time string hashing.
- Use `std::index_sequence` to **unroll a loop at compile time**.

🔹 **Metaprogramming**
- Implement a **variadic template-based** dispatcher for market events.
- Write a compile-time **state machine** for order execution.

---

## **6️⃣ Networking & Distributed Systems**
🔹 **Zero-Copy Techniques**
- Implement a **zero-copy** shared memory IPC mechanism.
- Optimize **TCP packet parsing** using **scatter-gather I/O**.

🔹 **High-Performance Networking**
- Implement a **UDP multicast receiver** for real-time market data.
- Design a **low-latency pub-sub system**.

---

## **7️⃣ Debugging & Profiling**
🔹 **Cache Miss Profiling**
- Given a program with frequent **cache misses**, optimize the memory layout.
- Profile the impact of **TLB misses** using `perf`.

🔹 **Lock Contention Profiling**
- Find and fix **lock contention issues** in a multi-threaded trading engine.

🔹 **GDB & LLDB Debugging Challenges**
- Debug a **race condition** using `rr` (Record & Replay Debugging).
- Use `gdb` to **trace memory corruption** in a large codebase.

---

## **8️⃣ Real-World HFT Scenarios**
🔹 **Latency Sensitivity**
- Optimize an order placement strategy to reduce **end-to-end latency**.
- Measure the **deterministic latency** of a system using **rdtsc**.

🔹 **Arbitrage Strategy Simulation**
- Implement a **multi-exchange arbitrage detector**.

🔹 **Market Data Snapshot Recovery**
- Reconstruct an **order book snapshot** from **incremental updates**.

---

## **📌 Final Thoughts**
- HFT **prioritizes low latency, memory efficiency, and concurrency**.
- Be **fluent in profiling tools** (`perf`, `VTune`, `gdb`).
- Expect **coding + system design** questions in **real-time trading scenarios**.



# Advanced
### 1. **Implement a custom memory allocator**  
   - **Question:** Implement a custom memory allocator (i.e., a simple version of `new`/`delete`) and manage dynamic memory allocation in C++ using `malloc`/`free` or `std::allocator`. Handle scenarios like fragmentation.
   - **Skills Tested:** Memory management, C++ object model, custom memory management.

### 2. **Implement an LRU Cache**  
   - **Question:** Design and implement a Least Recently Used (LRU) Cache. The cache should support the following operations:  
     - `get(key)` - Returns the value of the key if the key exists in the cache, otherwise return -1.
     - `put(key, value)` - Inserts the value if the key does not exist. If the cache is full, it should remove the least recently used item before inserting the new item.
   - **Skills Tested:** Data structures (hash map + doubly linked list), algorithm design.

### 3. **Concurrency: Implement a thread-safe queue**  
   - **Question:** Implement a thread-safe queue that supports both producers and consumers, using C++11 threads and synchronization primitives like `mutex` and `condition_variable`.
   - **Skills Tested:** Concurrency, thread synchronization, C++ standard library.

### 4. **Find the longest subsequence with a given sum**  
   - **Question:** Given a sequence of integers and a target sum, find the longest subsequence where the sum of the subsequence is equal to the target sum.
   - **Skills Tested:** Dynamic programming, greedy algorithms, or sliding window.

### 5. **Solve the N-Queens problem**  
   - **Question:** Implement a solution to the N-Queens problem, which asks you to place N queens on an NxN chessboard such that no two queens threaten each other. Print all possible solutions.
   - **Skills Tested:** Backtracking, recursion, combinatorics.

### 6. **Design and implement a multithreaded producer-consumer system**  
   - **Question:** Implement a multithreaded system where a producer thread generates items and places them into a shared buffer (queue), and multiple consumer threads process the items. Handle thread synchronization.
   - **Skills Tested:** Concurrency, synchronization, thread communication.

### 7. **Implement a Trie (Prefix Tree)**  
   - **Question:** Design and implement a Trie data structure for storing a set of strings. Implement the insert, search, and delete operations. Additionally, implement a function that returns all words starting with a given prefix.
   - **Skills Tested:** Data structures, prefix search, recursive algorithms.

### 8. **Design a thread pool**  
   - **Question:** Implement a thread pool in C++ that can handle a queue of tasks. Each task should be processed by a worker thread from the pool. Implement thread-safe task scheduling and efficient resource management.
   - **Skills Tested:** Concurrency, task management, thread synchronization.

### 9. **Find the longest common subsequence (LCS)**  
   - **Question:** Given two strings, implement a function to find the length of the longest common subsequence. Use dynamic programming to optimize the solution.
   - **Skills Tested:** Dynamic programming, string manipulation.

### 10. **Implement Dijkstra's Algorithm**  
   - **Question:** Implement Dijkstra's algorithm to find the shortest path from a source node to all other nodes in a graph. Your solution should handle weighted edges and work efficiently with priority queues.
   - **Skills Tested:** Graph algorithms, priority queue (min-heap), Dijkstra’s shortest path.
# Intermidiate
Here are 10 commonly asked intermediate-level C++ interview questions, focusing on data structures, algorithms, and some key C++ language concepts. These questions are suitable for someone with a solid grasp of C++ but not necessarily at an advanced level.

### 1. **Reverse a Linked List**  
   - **Question:** Given a singly linked list, reverse it in place.
   - **Skills Tested:** Linked list manipulation, pointer handling, algorithmic thinking.

### 2. **Find the Middle Element of a Linked List**  
   - **Question:** Given a singly linked list, find the middle element in a single pass. If there are two middle elements, return the second one.
   - **Skills Tested:** Linked list, fast and slow pointer technique, algorithm design.

### 3. **Check for Palindrome**  
   - **Question:** Given a string, check if it is a palindrome (reads the same forward and backward).
   - **Skills Tested:** String manipulation, two-pointer technique, basic algorithms.

### 4. **Merge Two Sorted Arrays**  
   - **Question:** Given two sorted arrays, merge them into a single sorted array.
   - **Skills Tested:** Arrays, sorting, merging, algorithm design.

### 5. **Find Missing Number in an Array**  
   - **Question:** Given an array of size `n-1` containing integers from 1 to `n`, find the missing number.
   - **Skills Tested:** Array manipulation, mathematical optimization, XOR technique.

### 6. **Count Inversions in an Array**  
   - **Question:** Given an array of integers, count the number of inversions in it. An inversion is a pair of indices `(i, j)` such that `i < j` and `arr[i] > arr[j]`.
   - **Skills Tested:** Merge sort, inversion counting, sorting algorithms.

### 7. **Find First Non-Repeating Character in a String**  
   - **Question:** Given a string, find the first non-repeating character in it.
   - **Skills Tested:** String manipulation, hash maps (unordered_map), efficient searching.

### 8. **Implement a Stack Using Queues**  
   - **Question:** Implement a stack using two queues. Implement the `push`, `pop`, and `top` operations.
   - **Skills Tested:** Data structures, stack, queue, and algorithmic thinking.

### 9. **Calculate the Power of a Number**  
   - **Question:** Given a number `x` and an integer `n`, implement `x^n` (x raised to the power n) without using the built-in power function.
   - **Skills Tested:** Recursion, exponentiation by squaring, efficient algorithms.

### 10. **Find the Intersection Point of Two Linked Lists**  
   - **Question:** Given two singly linked lists, determine the point where they intersect. If they don't intersect, return `nullptr`.
   - **Skills Tested:** Linked list manipulation, pointer comparison, algorithm design.

# Common
Here are 10 commonly asked questions from the mentioned platforms that are designed for **intermediate and advanced C++ programmers**. These questions focus on algorithms, data structures, and core C++ concepts, commonly found in **written tests** on online platforms like LeetCode, HackerRank, Codeforces, etc.

### 1. **Reverse a Linked List**
   - **Platform:** LeetCode, HackerRank
   - **Description:** Given a singly linked list, reverse it in-place and return the reversed list.
   - **Skills Tested:** Linked list manipulation, pointer management, iterative/recursive techniques.
   - **Difficulty:** Medium

### 2. **Merge Intervals**
   - **Platform:** LeetCode
   - **Description:** Given a collection of intervals, merge all overlapping intervals and return an array of the non-overlapping intervals.
   - **Skills Tested:** Sorting, interval manipulation, greedy algorithms.
   - **Difficulty:** Medium

### 3. **Find the Kth Largest Element in an Array**
   - **Platform:** LeetCode, Codeforces
   - **Description:** Given an unsorted array, find the Kth largest element.
   - **Skills Tested:** Sorting, priority queues, quickselect algorithm.
   - **Difficulty:** Medium

### 4. **Maximum Subarray Sum (Kadane's Algorithm)**
   - **Platform:** HackerRank, LeetCode
   - **Description:** Given an integer array, find the contiguous subarray that has the largest sum.
   - **Skills Tested:** Dynamic programming, greedy algorithms.
   - **Difficulty:** Medium

### 5. **Implement LRU Cache**
   - **Platform:** LeetCode, HackerRank
   - **Description:** Design and implement an LRU (Least Recently Used) cache system.
   - **Skills Tested:** Hash maps, doubly linked list, cache eviction strategy.
   - **Difficulty:** Hard

### 6. **Unique Paths in a Grid**
   - **Platform:** LeetCode
   - **Description:** Given a m x n grid, find the number of unique paths from the top-left to the bottom-right corner.
   - **Skills Tested:** Dynamic programming, combinatorics.
   - **Difficulty:** Medium

### 7. **Find All Anagrams in a String**
   - **Platform:** LeetCode, HackerRank
   - **Description:** Given a string and a pattern, find all occurrences of the pattern’s anagrams in the string.
   - **Skills Tested:** Sliding window, hashmaps, string manipulation.
   - **Difficulty:** Medium

### 8. **Detect Cycle in a Graph (Using DFS or BFS)**
   - **Platform:** HackerRank, Codeforces
   - **Description:** Given a directed/undirected graph, determine whether a cycle exists.
   - **Skills Tested:** Graph traversal, depth-first search (DFS), breadth-first search (BFS).
   - **Difficulty:** Medium

### 9. **Longest Increasing Subsequence**
   - **Platform:** LeetCode, Codeforces
   - **Description:** Given an array of integers, find the length of the longest strictly increasing subsequence.
   - **Skills Tested:** Dynamic programming, binary search, greedy algorithms.
   - **Difficulty:** Medium

### 10. **String to Integer (atoi)**
   - **Platform:** LeetCode
   - **Description:** Implement the function that converts a string to an integer, similar to the built-in `atoi` function in C++.
   - **Skills Tested:** String manipulation, handling edge cases (e.g., whitespace, invalid characters).
   - **Difficulty:** Easy to Medium
