# OrderBook Design
The specifics depend on if you're implementing for equities (order-based) or futures (level-based).
I recommend https://web.archive.org/web/20110219163448/http://howtohft.wordpress.com/2011/02/15/how-to-build-a-fast-limit-order-book/ for a general overview of a good architecture for the former.

Building off of that, though, I have found that using array-based over pointer-based data structures provides faster performance.
This is because for modern pipelining, caching CPUs, branching and dereferences (especially from uncached memory) are the biggest performance killers because they introduce data dependencies resulting in pipeline / memory stalls.

If you can, you should also optimize for the particular exchange.
For instance, it turns out that, last I checked, Nasdaq generates order IDs incrementally starting from a small number, so you can store all the orders in a giant array instead of a hashtable.
This is really cache- and TLB-friendly compared to a hashtable because most updates tend to happen to recently-dereferenced orders.

Speaking from experience with equities (order-based), my preferred architecture is:

A large associative array from order ids to order metadata (std::unordered_map or std::vector if you can swing it such as in the case of Nasdaq).
The order metadata includes pointers to the order book (essentially consisting of the price-levels on both sides) and price-level it belongs to, so after looking up the order,
the order book and price level data structures are a single dereference away.
Having a pointer to the price allows for a O(1) decrement for an Order Execute or Order Reduce operation.
If you want to keep track of time-priority as well, you can keep pointers to the next and previous orders in the queue.
Since most updates happen near the inside of the book, using a vector for the price levels for each book will result in the fastest average price lookup.
Searching linearly from the end of the vector is on average faster than a binary search, too,
because most of the time the desired price is only a few levels at most from the inside, and a linear search is easier on the branch predictor, optimizer and cache.
Of course, there can be pathological orders far away from the inside of the book, and an attacker could conceivably send a lot of updates at the end of the book in order to slow your implementation down.
In practice, though, most of the time this results in a cache-friendly, nearly O(1) implementation for insert, lookup, update and delete (with a worst-case O(N) memcpy).
Specifically for a Best Bid / Offer (BBO) update, you can get an implementation to calculate the update in just a few instructions (essentially appending or deleting an element from the end of the vector), or about three dereferences.
The behavior of this is O(1) best-case behavior for insertion, lookup, delete and update with very low constants (I've clocked it to be on average the cost of a single fetch from main memory - roughly 60ns).
Unfortunately you can get O(N) worst case behavior, but with low probability and still with very good constants due to the cache-, TLB- and compiler-friendliness.
It is also very fast, nearly optimally so, in the case of BBO updates which is what you are usually interested in anyways.
I have a reference implementation here for Nasdaq's ITCH protocol which illustrates these techniques and more.
It clocks in at a mean of just over 61ns / tick (about 16 million ticks / second) by only keeping track of the quantity at each price-level and not the order queue,
and uses almost no allocation besides std::vector resizing.

## Breakdown of the Specification in Smaller Steps with Examples:

**1. Performance Focus and Data Structures:**

  - The provided text emphasizes achieving fast performance for order book operations (insertion, deletion, lookup, update).
  - It argues for using array-based data structures (like vectors) over pointer-based ones (like linked lists) due to cache efficiency and reduced branching.

**2. Exchange-Specific Optimizations:**

  - The specification acknowledges that different exchanges might have different order book structures.
    Here, it focuses on equities (order-based) but mentions futures (level-based) as an alternative.
  - As an example, it suggests that for Nasdaq, where order IDs are likely consecutive integers, storing orders in a large array might be more efficient than a hash table.
    This leverages cache-friendliness as recently accessed orders are likely to be close in memory.

**3. Order Metadata and Data Pointers:**

  - The preferred architecture proposes using an associative array (like `std::unordered_map`) to link order IDs with order metadata.
    This metadata could include pointers to the order's corresponding price level within the order book.
  - This allows for quick access to both the order details and its position in the price level structure using a single dereference.

**4. Order Execution and Time Priority:**

  - The text mentions that order execution follows a First-In, First-Out (FIFO) principle.
    This means the oldest order at a specific price level gets executed first.
  - To maintain time priority, the specification suggests storing orders within a price level using a data structure that allows efficient deletion from the beginning (like a queue).

**5. Price Level Data Structure and Lookups:**

  - The document recommends using vectors for storing price levels within the order book.
    Vectors provide fast average price lookups since searching happens linearly from the end (where new levels are added).
  - This approach is considered faster than a binary search in most practical scenarios as desired prices are often close to the "inside" (best bid and offer) of the book.
  - The text acknowledges potential drawbacks like slowdowns caused by pathological orders placed far from the inside or malicious attempts to manipulate lookups.

**6. Overall Performance and Trade-offs:**

  - Despite potential worst-case scenarios with O(N) complexity (e.g., deleting from the middle of a vector),
    the specification highlights the benefits of this approach for achieving near-constant time (O(1)) behavior for most operations.
  - It emphasizes cache-friendliness and low overhead for typical use cases.
    Finally, it mentions a reference implementation with impressive performance benchmarks.

**Examples:**

* **Order Book with Vectors:** Imagine an order book where each price level is represented by a vector.
    Adding a new order involves pushing it to the back of the corresponding price level vector (O(1)).
    Deleting an order, especially if it's the oldest (relevant for execution), would require shifting elements in the vector, resulting in O(N) complexity in the worst case.
* **Order Metadata with Pointers:** Consider an order metadata structure that stores a pointer to the price level where the order resides.
    Looking up the order's position in the book then involves a single dereference of this pointer, offering constant time access (O(1)).

**Remember:** This breakdown focuses on the key points of the specification. It's crucial to analyze your specific use case and workload to determine the optimal data structures and optimizations for your order book implementation.
Here are some approaches to improve the cache-friendliness and determinism of hashing in the context of order books:

**Cache-Friendly Hashing:**

* **Custom Hash Function:** Consider designing a custom hash function that incorporates information about the price level data structure. For example, if `PriceLevel` objects are stored contiguously in memory, the hash function could leverage bitwise operations on the memory address of the `PriceLevel` to generate a hash value. This can potentially improve cache utilization by placing related price levels close together in the hash table.

* **Padding PriceLevel Objects:** If the size of `PriceLevel` objects is not a multiple of the cache line size, consider padding them with dummy elements. This ensures multiple price levels can be stored within a single cache line, improving utilization.

**Deterministic Branching:**

* **Robin Hood Hashing:** While `std::unordered_map` uses a linear probing collision resolution strategy, you could explore alternative hashing techniques like Robin Hood hashing. This approach aims to distribute elements more uniformly within the hash table, potentially reducing the number of probes needed during lookup and insertion, leading to more predictable branching behavior.

* **Separate Chaining with Fixed-Size Lists:**  Standard separate chaining uses linked lists to store elements with the same hash value. You could implement a variation with fixed-size small arrays (e.g., size 2 or 4) instead of linked lists. This provides a more predictable access pattern compared to potentially long linked list traversals.

**Trade-offs and Considerations:**

* Implementing custom hash functions or alternative collision resolution strategies might add complexity to your code.
* Padding objects and using fixed-size arrays can increase memory usage.
* The effectiveness of these techniques depends on factors like the access patterns in your specific application and the underlying hardware architecture.

**General Recommendations:**

* Profiling your order book implementation with real-world data is crucial to identify performance bottlenecks.
* Measure the impact of cache-friendly hashing and deterministic branching techniques on your specific workload.
* The benefits of these optimizations might be less significant if your order book operations are dominated by random insertions/deletions or if the data volume is small enough to fit comfortably in cache.

**In conclusion:**

While `std::unordered_map` offers good overall performance for order books, exploring custom hashing or alternative collision resolution strategies can potentially improve cache utilization and determinism of branching behavior. However, these optimizations should be carefully evaluated based on your specific use case and workload to weigh the benefits against potential complexity and memory overhead.
