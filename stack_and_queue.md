# How would you design a stack which, in addition to `push` and `pop`, also has a function `min` which returns the minimum element? All operations should be O(1).
## Algorithm: Stack with min in O(1) time

**Objective:** Design a stack data structure that supports the following operations in constant time (O(1)):

- `push(x)`: Pushes element `x` onto the stack.
- `pop()`: Removes and returns the top element from the stack.
- `min()`: Returns the minimum element currently in the stack.

**Data Structure:**

We will use two stacks:

1. **Main Stack (`dataStack`):** This stack stores the actual data elements pushed onto the stack.
2. **Minimum Stack (`minStack`):** This stack keeps track of the minimum elements encountered so far.

**Idea:**

The `minStack` will always hold the minimum element currently present in the `dataStack`. We achieve this by carefully managing the elements pushed and popped from both stacks.

**Algorithm:**

1. **`push(x)`:**
    - Push `x` onto the `dataStack`.
    - If `minStack` is empty OR `x` is less than or equal to the top element of `minStack`:
        - Push `x` onto `minStack` as well.

2. **`pop()`:**
    - Pop the top element from the `dataStack` (let's call it `top`).
    - If `top` is also equal to the top element of `minStack`:
        - Pop the top element from `minStack` as well (to maintain the minimum element in `minStack`).

3. **`min()`:**
    - Simply return the top element of the `minStack`.

## Time and Space Complexity:
**Time Complexity:**

 - `push(x)` : O(1) - Constant time operation for pushing to both stacks   (if needed).
 - `pop()`   : O(1) - Constant time operation for popping from both stacks (if needed).
 - `min()`   : O(1) - Constant time operation to access the top element of `minStack`.

**Space Complexity:**

- O(n) - In the worst case, the `minStack` might store all elements if they are added in increasing order. However, the space usage depends on the input sequence.

**Explanation:**

By maintaining the minimum element in a separate stack, we can retrieve it in constant time.
Pushing and popping involve manipulating both stacks, but these operations are still constant time as they only involve basic stack operations.

The `minStack` will not necessarily hold only one element, but it will always hold the **minimum value** encountered so far in the `dataStack`.

Here's a clarification:

- **Size of `minStack`:** The size of `minStack` can vary depending on the sequence of elements pushed.
    - In the worst case, if elements are pushed in increasing order, `minStack` will grow with each push, potentially holding all elements in the `dataStack`.
    - However, if elements are pushed in a way where the minimum value keeps changing, `minStack` might only hold a few elements (or even just one) at any given time.
- **Maintaining Minimum Value:** The key point is that the top element of `minStack` always represents the **current minimum value** in the `dataStack`. This allows us to retrieve the minimum element in constant time using `min()`.

## Example to illustrate:

**Push Sequence:** 6, 9, 2, 4, 7

**State of Stacks:**

| Operation | dataStack (Top -> Bottom) | minStack (Top -> Bottom) |
|---|---|---|
| push(6) | 6 | 6 |
| push(9) | 6, 9 | 6 | (Minimum remains 6)
| push(2) | 6, 9, 2 | 6, 2 | (New minimum pushed)
| push(4) | 6, 9, 2, 4 | 6, 2 | (Minimum remains 2)
| push(7) | 6, 9, 2, 4, 7 | 6, 2 | (Minimum remains 2)

Here, you can see the complete history of minimum values encountered:

- Initially, only 6 is in `minStack`.
- When 2 is pushed (being the new minimum), both 6 (previous minimum) and 2 are pushed onto `minStack`.
- Since 4 and 7 are not smaller than 2, `minStack` remains unchanged.

This representation provides a clearer picture of how `minStack` keeps track of minimum values throughout the push operations.
In this example, `minStack` never holds more than one element at a time, but it always reflects the current minimum value in the `dataStack`.

# Discuss the use of queues in managing trade orders and other tasks in trading systems.
Queues are widely used in managing trade orders due to their First-In-First-Out (FIFO) structure, making them well-suited for processing orders in the sequence they are received. Here's a breakdown of their usage:
## Why use queues?
    Queues are used to manage the flow of trade orders in a way that is efficient, reliable, and scalable.
    They allow for the processing of large volumes of orders in a way that is predictable and consistent.

  - **Order Processing :** Orders are placed by traders and queued for execution. This ensures a fair and orderly market where orders are filled based on their arrival time.
  - **Sequencing       :** Queues guarantee that buy and sell orders for a specific security are matched in the order they were received.
  - **Prioritization   :** Different types of orders (e.g., market orders vs. limit orders) can be prioritized within separate queues for a more controlled execution flow.

## Types of queues:
### Single Queue:
    All orders for a security are placed in a single queue, ensuring strict FIFO processing.
### Priority Queues:
    Different order types (e.g., market vs. limit) or price ranges can have dedicated queues for prioritized execution based on pre-defined rules.
    **Priority in Trading Queues:**
        In these dedicated queues, priority is often determined by:
        - **Order Type  :** Market orders might have higher priority than limit orders, ensuring faster execution.
        - **Price Range :** Within the same order type (e.g., limit orders), orders with more aggressive prices (closer to the current market price) might have higher priority for matching.
    **Implementation Details:**
        These dedicated queues likely use a **priority queue** data structure internally.
        A priority queue ensures that the element with the highest priority (based on the pre-defined rules) is always at the front of the queue.
        Here's how it works:
            - For queues based on order type (market vs. limit), the priority could be a simple flag (True for market orders, False for limit orders).
              The queue would then be structured to prioritize elements with a True flag (market orders) over those with False (limit orders).
            - For queues based on price range, the priority might be assigned based on the absolute difference between the order price and the current market price.
              Orders with a smaller difference (closer price) would have higher priority for matching.
    **Key Point:**
        While each dedicated queue maintains its own internal priority order, it's important to remember that there might be **multiple such queues** for different order types or price ranges.
        The overall order of execution might involve processing orders from these queues based on a higher-level prioritization scheme defined by the trading system.

### Dequeues:
  Technically, a dequeue (double-ended queue) is a type of queue, but it's a specialized version that allows for insertion and deletion from both ends (front and back) unlike a standard queue which is FIFO (First-In-First-Out).

  Here's a breakdown of dequeues, their implementation, and complexity:

#### Deque Implementation:

  There are two common ways to implement deques:

##### Doubly Linked List:** This is a popular approach where elements are linked together using forward and backward pointers.
      This allows for efficient insertion and deletion from both ends (O(1) time complexity) as you only need to manipulate the pointers.
      However, random access (e.g., accessing an element by index) becomes slower (O(n) time complexity) compared to an array-based implementation.

###### Algorithm to create dequed using doubly linked-list.
  **1. push_front (Insert at Front):**
    1. **Create a new node:** Allocate memory for a new `Node` object and initialize its `data` field with the value to be inserted. Set the `prev` and `next` pointers of the new node to `nullptr` initially.
    2. **Empty Deque:** If the deque is empty (both `head` and `tail` are `nullptr`), set both `head` and `tail` to point to the newly created node.
    3. **Non-Empty Deque:**
       - Set the `next` pointer of the new node to the current `head` node.
       - Update the `prev` pointer of the current `head` node to point to the new node.
       - Update the `head` pointer to point to the new node, making it the new front of the deque.

  **2. push_back (Insert at Back):**
    1. **Create a new node:** Similar to `push_front`, allocate memory and initialize the data field of the new node.
    2. **Empty Deque:** If the deque is empty, set both `head` and `tail` to point to the newly created node.
    3. **Non-Empty Deque:**
      - Set the `prev` pointer of the new node to the current `tail` node.
      - Update the `next` pointer of the current `tail` node to point to the new node.
      - Update the `tail` pointer to point to the new node, making it the new back of the deque.

  **3. pop_front (Remove from Front):**
    1. **Empty Deque:** If the deque is empty, print an error message or throw an exception (implementation dependent).
    2. **Single Element Deque:** If there's only one element (`head` and `tail` point to the same node), set both `head` and `tail` to `nullptr` to indicate an empty deque. Store the data value of the node for returning.
      - Deallocate the memory associated with the removed node.
    3. **Multiple Elements Deque:**
      - Store the data value of the current `head` node for returning.
      - Update the `head` pointer to point to the `next` node of the current `head`.
      - Update the `prev` pointer of the new `head` node to set it to `nullptr` as it's now the front of the deque.
      - Deallocate the memory associated with the removed node (the old `head`).

  **4. pop_back (Remove from Back):**
    1. **Empty Deque:** Similar to `pop_front`, handle the empty deque case with an error message or exception.
    2. **Single Element Deque:** Identical to the single element case in `pop_front`. Set both `head` and `tail` to `nullptr` and store/deallocate the node.
    3. **Multiple Elements Deque:**
      - Store the data value of the current `tail` node for returning.
      - Update the `tail` pointer to point to the `prev` node of the current `tail`.
      - Update the `next` pointer of the new `tail` node to set it to `nullptr` as it's now the back of the deque.
      - Deallocate the memory associated with the removed node (the old `tail`).

  These algorithms highlight the advantage of a doubly linked list for deque operations.
  By maintaining both forward and backward pointers in each node, we can efficiently insert and delete elements from both ends of the deque in constant time (O(1) amortized time complexity).


##### Resizable Array:** This approach uses an array that can dynamically resize as needed.
      It allows for faster random access (O(1) on average) but might incur some overhead for resizing operations.
      Insertion and deletion at the ends are still efficient (O(1) amortized time complexity), meaning the average time per operation remains constant even with frequent insertions/deletions.

  The C++ standard library's `deque` class uses a variant of the resizable array implementation with optimizations for efficient insertion and deletion at both ends.
#### Time and space complexity:

  **Time Complexity:**
      - **Insertion (push_front/push_back):** O(1) amortized time complexity (efficient on average)
      - **Deletion (pop_front/pop_back):** O(1) amortized time complexity
      - **Access (operator[]):** O(1) on average, might be slower in the worst case for a linked list implementation.
      - **Search:** O(n) time complexity, as you might need to traverse the entire queue in the worst case.

  **Space Complexity:**
      - The space complexity of a deque is typically O(n), where n is the number of elements in the queue. This is because it needs to store the data for each element and the pointers or array size for efficient operations.

  **C++ Standard Library deque:**
      The C++ standard library's `deque` class provides the following functionalities:
      - **Insertion:** `push_front` (front), `push_back` (back)
      - **Deletion:** `pop_front` (front), `pop_back` (back)
      - **Access:** `operator[]` (by index)
      - **Size:** `size()` (number of elements)
      - **Empty:** `empty()` (checks if empty)
#### Common Dequeue Applications in Trading Systems:
    **Dequeues are not as commonly used in core trade order management** compared to priority queues.
    However, they can be utilized for specific tasks within a trading system:
    Dequeues are more commonly used for other functionalities within a trading system:
##### Market Data Feed Management:
    Dequeues can be employed to buffer incoming market data feeds (e.g., stock prices, order book updates) in a FIFO manner.
    This allows for efficient processing of the data while maintaining the order of arrival.
##### Order Routing/Aggregation:
    Dequeues can be used for temporary storage and transmission of orders to different exchanges or execution venues.
    Orders can be added to the front of the queue for prioritization or the back for FIFO processing depending on the routing strategy.
##### Risk Management and Algorithmic Trading:
    Dequeues can be used to store and manage historical data for risk calculations or algorithmic trading strategies.
    These strategies might require accessing data points in the order they were received (FIFO) for calculations or analysis.

### Multi-level Queues:
    A combination of single and priority queues can be used, offering flexibility in order processing.

## Efficiency:
- **Simplicity  :** Implementing and managing queues are relatively simple and efficient.
- **Fairness    :** Queues guarantee a fair market by processing orders in the arrival order.
- **Scalability :** Queues can handle high volumes of orders efficiently by adding new orders to the back of the queue.

## Time and Space Complexity:

### Time Complexity:
  **FIFI Queue:**
    - **Enqueue (Adding order)   :** O(1) - Constant time operation as it adds to the end of the queue.
    - **Dequeue (Removing order) :** O(1) - Constant time operation as it removes from the front of the queue.
    - **Search                   :** O(n) - Linear time complexity if searching for a specific order within the queue (not a common operation).

  **Priority Queue:**
    - **Enqueue (Adding element)       :** O(log n) - In most implementations, adding an element to a priority queue involves maintaining the heap property (for max-heaps or min-heaps), which takes logarithmic time in the number of elements (n).
    - **Dequeue (Removing top element) :** O(log n) - Removing the top element (highest/lowest priority depending on the implementation) also requires maintaining the heap property, resulting in logarithmic time complexity.
    - **Peek (Accessing top element)   :** O(1) - Retrieving the top element without removing it usually involves just accessing the root node of the heap, making it a constant time operation.
    - **Search                         :** O(n) - Searching for a specific element within the priority queue typically involves iterating through all elements, leading to linear time complexity. This operation is not as common as enqueue, dequeue, or peek.

### Space Complexity:
  **FIFI Queue:**
    **Overall:** O(n) -  The queue's memory usage grows linearly with the number of orders in it.
  **Priority Queue:**
    **Overall:** O(n) -  A priority queue typically uses a heap data structure to store elements. A heap can be represented using an array, and the space complexity grows linearly with the number of elements (n) in the queue.


## Optimization:
    Queues can be optimized in several ways to improve their performance.
### Space:
#### Bounded Queues:
    Limit the maximum size of the queue to prevent excessive memory usage during high-volume periods. Expired or low-priority orders can be dropped from the queue.
#### Circular Queues:
    Utilize a circular structure within a fixed-size memory block, overwriting older entries when the queue is full. This can improve the performance of the queue by reducing the number of memory allocations and deallocations required.
### Time:
#### Priority Processing:
##### Priority Queue:
    A priority queue itself can be used to achieve similar results as pre-sorting a queue based on order type.
##### Pre-sorting:
  - **Pre-Sorting:** This is an additional step performed **before** utilizing the priority queue for order matching. It involves sorting the entire queue based on a specific priority (e.g., order type) **outside** of the priority queue structure.

  **Why Pre-Sort?**

  - **Faster Processing:** If specific order types (e.g., market orders) need faster execution, pre-sorting the entire queue based on order type ensures that market orders appear at the beginning.
    This allows for quicker access and processing of these high-priority orders.

  **The Trade-off:**
      While pre-sorting offers faster access to specific order types, it comes with a trade-off:
      - **Overhead:** Sorting the entire queue adds an extra processing step before utilizing the built-in priority handling of the queue itself.
        This additional sorting might be less efficient for scenarios where the priority within the queue is already sufficient.

  **Alternatives:**
      - **Multiple Priority Queues:** Instead of pre-sorting, one approach could involve using separate priority queues for different order types. This eliminates the need for pre-sorting but increases memory usage due to maintaining multiple queues.
      - **Hybrid Approach:** Depending on the system requirements, a hybrid approach could be employed. The main queue might be a standard priority queue handling order types internally, while a pre-sorted sub-queue could be maintained for market orders for faster access.
  **Conclusion:**
      Pre-sorting is a technique to optimize order processing for specific order types within a queue system.
      It doesn't negate the use of a priority queue within the system. Both approaches can be used together or independently depending on the specific needs of the trading system.

###### An example of pre-sorting a queue before order matching:

  **Scenario:**
  Imagine a queue holding various trade orders, including market orders (high priority) and limit orders (lower priority). We want to ensure market orders are processed faster.

  **Step 1: Unsorted Queue**
  ```
  Queue: [Limit Order (Price: 100), Market Order, Limit Order (Price: 95), Market Order, Limit Order (Price: 101)]
  ```

  **Step 2: Pre-Sorting based on Order Type (Market Orders first)**
  ```
    Pre-sort based on Order Type (Market Orders first)
  ```

  ```
  Queue: [Market Order, Market Order, Limit Order (Price: 100), Limit Order (Price: 95), Limit Order (Price: 101)]
  ```

  **Step 3: Order Matching (using the pre-sorted queue)**
  The trading system can now process orders from the front of the queue, ensuring market orders are matched first due to the pre-sorting step.

  **Explanation:**
  - The initial queue holds a mix of order types.
  - We pre-sort the queue based on the priority (Market Orders first), placing them at the beginning.
  - Now, when the system starts processing orders, it encounters market orders first, fulfilling the need for faster execution of these high-priority orders.

  **Points to Remember:**
  - Pre-sorting is an additional step before utilizing the queue for matching orders.
  - It adds some overhead in terms of sorting time but can significantly improve processing speed for high-priority orders.
  - This is a simplified example, and real-world trading systems might use more complex priority schemes within the queue itself or employ a combination of pre-sorting and internal queue priorities.

##### Pre-sorting vs Priority Queue Trade-Off:
    Here's a breakdown of both approaches and their trade-offs:

  **Pre-sorting:**
  - **Advantage:** It guarantees that high-priority orders (e.g., market orders) are placed at the beginning of the queue, leading to faster access and processing.
  - **Disadvantage:** Sorting the entire queue adds an extra processing step before utilizing the queue for matching orders. This sorting overhead can be significant for large queues.

  **Priority Queue based on Order Type:**
  - **Advantage:** No need for explicit pre-sorting. The priority queue itself maintains the order based on the defined priority (order type in this case).
  - **Disadvantage:** The internal operations of a priority queue might involve some overhead compared to a simple pre-sort based on a flag (market vs. limit). However, this overhead is usually much smaller compared to full queue sorting.

  **Trade-off Summary:**
  | Approach | Advantage | Disadvantage |
  |---|---|---|
  | Pre-sorting | Guaranteed order for high-priority elements | Additional sorting overhead |
  | Priority Queue (Order Type) | No pre-sorting needed | Potentially slightly slower internal operations compared to pre-sorting |

  **Choosing the Right Approach:**
  The best approach depends on the specific needs of the trading system:
  - **High Volume and Performance:** If the system deals with high-volume order processing and needs absolute speed for high-priority orders, **pre-sorting** might be preferred despite the overhead.
  - **Balance and Simplicity:** If the priority within the queue itself is sufficient, and sorting overhead is a concern, using a **priority queue based on order type** offers a good balance between simplicity and efficiency.

  **Additional Considerations:**
  - **Hybrid Approach:** In some cases, a hybrid approach could be beneficial. The main queue might be a standard priority queue handling order types internally, while a pre-sorted sub-queue could be maintained for market orders for faster access.
  - **Complexity vs. Performance:** Ultimately, the choice depends on factors like the complexity of the priority scheme (beyond just order type), system load, and performance requirements.

  **In conclusion,** both pre-sorting and priority queues are viable options for handling order types in a trading system. Understanding their trade-offs allows you to select the most suitable approach for your specific needs. 

#### Multi-threaded Processing:
    Implement multiple threads to handle order processing from different queues concurrently.

## Concurrency:
    - Queues face challenges in concurrent environments where multiple processes (threads) might access the queue simultaneously.
    This can lead to race conditions and data corruption.

## Thread Safety:
    - **Mutexes:** Use a mutex (mutual exclusion) lock to ensure only one thread can access the queue for operations (enqueue/dequeue) at a time. While effective, this can introduce performance overhead.
    - **Lock-free Queues:** Employ specialized concurrent queue implementations like lock-free queues that use atomic operations to avoid mutexes, improving concurrency with careful design.

## Alternatives to Queues:
### Priority Trees:
    While more complex to manage, priority trees offer faster search and retrieval functionalities when specific order matching is crucial.
### Order Books:
    Maintain separate buy and sell order lists sorted by price for efficient matching of buy and sell orders at specific price levels.
#### Order Book Design
  An order book is a fundamental data structure in a trading system that maintains buy and sell orders for a specific financial instrument (stock, currency, etc.).
  It facilitates efficient matching of buy and sell orders at specific price levels, ultimately determining the market price for the instrument.

  Here's a breakdown of the design, data structure, and operations of an order book:

##### Design
    - **Two Main Lists** : The order book typically uses two separate lists, one for **buy orders** (bids) and one for **sell orders** (asks) sorted by price level.
    - **Sorting** : Within each list, orders are sorted with the **highest price for buy orders** (willingness to pay) at the front and the **lowest price for sell orders** (asking price) at the front.
        This allows for efficient matching of orders with the best possible price for the buyer or seller.
##### Data Structure Options :
    Each list can be implemented using various data structures like:
    - **Arrays** : Offer fast random access but might be less efficient for frequent insertions/deletions.
    - **Linked Lists** : Efficient for insertions/deletions but slower for random access (e.g., finding the best bid/ask).
    - **Self-balancing Binary Search Trees (e.g., AVL Trees)** : Offer a balance between insertion/deletion efficiency and search/access speed. This is a popular choice for order books.
###### Selft-balancing binary search tree:
    The self-balancing binary search trees (like AVL trees) used in order books are indeed based on price comparison for sorting and searching. Here's a breakdown:
    **Price as the Key:**
        - Each order in the order book (buy or sell) acts as a node in the binary search tree.
        - The key value used for comparison and sorting within the tree is the **order price**.
          - For **buy orders:**
            - The comparison logic prioritizes higher prices.
              A higher price for a buy order means the buyer is willing to pay more, so it gets placed higher in the tree (closer to the root).
          - For **sell orders:**
            - The comparison prioritizes lower prices.
              A lower price for a sell order means the seller wants to get rid of the instrument quicker, so it gets placed lower in the tree.
    **Matching and Order Book Depth:**
        - This price-based sorting allows for efficient retrieval of the **best bid** (highest price for a buy order) and the **best ask** (lowest price for a sell order).
          These are typically located at the root of the buy and sell trees, respectively.
        - The order book might only display a limited number of price levels on each side (buy and sell) to provide a general picture of supply and demand.
          This is known as the **order book depth**.
    **Example (Simplified):**
        Imagine an order book with buy and sell orders for a stock:
        ```
        Buy Orders (Price: Quantity)
            - $10.20: 100 (Root of Buy Tree)
            - $10.15: 200
            - $10.00: 50
        Sell Orders (Price: Quantity)
            - $10.25: 300 (Root of Sell Tree)
            - $10.30: 150
            - $10.35: 400
        ```

    In this example, the AVL trees sort the orders based on price.
    The root of the buy tree holds the highest buy order ($10.15), and the root of the sell tree holds the lowest sell order ($10.30).
    This allows for efficient retrieval of the best bid and ask.
    The order book might only show a limited depth (e.g., top 2-3 levels on each side) depending on the implementation.

##### Operations:
    - **Add Order:** This operation takes an order (buy or sell) and inserts it into the appropriate list (buy or sell) while maintaining the price-based sorting.
    - **Cancel Order:** This operation removes a specific order from the appropriate list based on an identifier (e.g., order ID).
    - **Modify Order:** This operation removes an existing order and then inserts a new order with the updated details (e.g., quantity, price) while maintaining the sorting.
    - **Match Orders:** This is a core operation where the order book identifies buy and sell orders at matching prices.
        Orders are matched starting from the best prices (highest buy and lowest sell). The system might execute trades based on these matches, potentially reducing the available quantity at each price level.
    - **Get Best Bid/Ask:** This operation retrieves the price and quantity of the top buy order (highest bid) and the top sell order (lowest ask).
      This information is crucial for market participants to understand the current supply and demand.

##### Additional Considerations:
    - **Order Levels (Price Aggregation):** In some order book implementations, instead of storing individual orders, the system might aggregate orders at specific price levels.
        This can save space and improve performance but might lose some granularity in order data.
    - **Order Book Depth:** The order book typically shows a limited number of price levels on each side (buy and sell) to provide a general picture of supply and demand.
        This is known as the order book depth.

##### Benefits of Order Book Design:
    - **Efficient Matching:** By sorting orders based on price, the system can efficiently match buy and sell orders for optimal execution.
    - **Price Discovery:** The order book reflects the current supply and demand, helping determine the market price for the instrument.
    - **Transparency:** Order books provide some level of transparency into market depth and potential trading activity.

##### Conclusion:
    Order books are essential components of trading systems, facilitating efficient order matching, price discovery, and market transparency.
    Understanding their design, data structures, and operations is crucial for anyone involved in trade order execution or algorithmic trading.
    **Choosing the right approach depends on factors like:**
        - Order volume and processing speed requirements
        - Need for order prioritization
        - System concurrency and thread safety considerations

