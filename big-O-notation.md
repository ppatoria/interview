# A list of data structures provided by STL with their average/best/worst BigO notation and scenario, along with the operations they support:

| **Data Structure** | **Average Case** | **Best Case** | **Worst Case** | **Scenario** | **Operations** |
|-----------------|----------------|-------------|---------------|-------------|----------------|
| Vector          | O(n)           | O(1)        | O(n)          | When you need a dynamic array. | Search: O(n), Insert: O(n), Delete: O(n), Resize: O(n) |
| List            | O(n)           | O(1)        | O(n)          | When you need a doubly linked list. | Search: O(n), Insert: O(1), Delete: O(1), Resize: N/A |
| Deque           | O(1)           | O(1)        | O(1)          | When you need a double-ended queue. | Search: O(n), Insert: O(1), Delete: O(1), Resize: O(n) |
| Set             | O(log n)       | O(1)        | O(log n)      | When you need a set of unique elements. | Search: O(log n), Insert: O(log n), Delete: O(log n), Resize: N/A |
| Map             | O(log n)       | O(1)        | O(log n)      | When you need a set of key-value pairs. | Search: O(log n), Insert: O(log n), Delete: O(log n), Resize: N/A |
| Stack           | O(1)           | O(1)        | O(1)          | When you need a last-in, first-out (LIFO) data structure. | Search: O(n), Insert: O(1), Delete: O(1), Resize: N/A |
| Queue           | O(1)           | O(1)        | O(1)          | When you need a first-in, first-out (FIFO) data structure. | Search: O(n), Insert: O(1), Delete: O(1), Resize: N/A |
| Priority Queue  | O(log n)       | O(1)        | O(log n)      | When you need a queue where the element with the highest priority is always at the front. | Search: O(n), Insert: O(log n), Delete: O(log n), Resize: N/A |
| Unordered Set   | O(1)           | O(1)        | O(n)          | When you need a set of unique elements. | Search: O(1), Insert: O(1), Delete: O(1), Resize: O(n) |
| Unordered Map   | O(1)           | O(1)        | O(n)          | When you need a set of key-value pairs. | Search: O(1), Insert: O(1), Delete: O(1), Resize: O(n) |

The extended information:

| **Data Structure** | **Underlying Data Structure** | **Worst Case Scenario** |
|-----------------|----------------|----------------|
| Vector | Dynamic Array | When you try to insert or delete an element not at the end. This is because all elements after the inserted/deleted element have to be shifted. |
| List | Doubly Linked List | When you try to search for an element. This is because you may have to traverse the entire list to find the element. |
| Deque | Double-Ended Queue | When you try to search for an element. This is because you may have to traverse the entire deque to find the element. |
| Set | Binary Search Tree (typically a self-balancing BST like Red-Black Tree) | When the tree becomes unbalanced and degenerates to a linked list. |
| Map | Binary Search Tree (typically a self-balancing BST like Red-Black Tree) | When the tree becomes unbalanced and degenerates to a linked list. |
| Stack | Linked List or Array | When you try to search for an element. This is because you may have to pop all elements until you find the element. |
| Queue | Linked List or Array | When you try to search for an element. This is because you may have to dequeue all elements until you find the element. |
| Priority Queue | Binary Heap | When you try to search for an element. This is because you may have to traverse the entire heap to find the element. |
| Unordered Set | Hash Table | When there are hash collisions. This is because elements with the same hash are stored in a linked list and you may have to traverse the entire list to find the element. |
| Unordered Map | Hash Table | When there are hash collisions. This is because elements with the same hash are stored in a linked list and you may have to traverse the entire list to find the element. |

# deque
  A double-ended queue, or deque, is a data structure that allows you to insert and remove elements from both ends (front and back) efficiently.
  This flexibility makes it useful in various scenarios:

  1. **Maintaining a Sliding Window**               : Deques are often used in algorithms that require maintaining a 'window' of elements in a larger dataset.
                                                      For example, finding the maximum element in all subarrays of size 'k' in an array.

  2. **Implementing a Queue with Stack Operations** : A deque can be used to implement a queue using only stack operations.
                                                      You can push elements to the back of the deque (equivalent to a stack push), and pop elements from the front of the deque (equivalent to repeatedly popping a stack until you get the bottom element).

  3. **Palindromes**                                : Deques can be used to check if a word is a palindrome (reads the same forward and backward).
                                                      You can continuously compare and remove the front and back elements of the deque, which should be equal for a palindrome.

  4. **Undo Functionality**                         : In text editors, a deque can be used to implement undo functionality.
                                                      The front of the deque can be used for the undo operation, and the back can be used for the redo operation.

# Some of the search algorithms provided by the Standard Template Library (STL) in C++:

  1. **std::find / std::find_if / std::find_if_not**
    - Average Case        : O(n)
    - Best Case           : O(1)
    - Worst Case          : O(n)
    - Best Case Scenario  : The element is at the beginning of the container.
    - Worst Case Scenario : The element is at the end of the container or not present.
    - Use Case            : When you need to find an element in a container and the order of elements is not sorted.

  2. **std::binary_search / std::lower_bound / std::upper_bound**
    - Average Case        : O(log n)
    - Best Case           : O(log n)
    - Worst Case          : O(log n)
    - Best Case Scenario  : The element is in the middle of the sorted container.
    - Worst Case Scenario : The element is at the beginning or end of the sorted container or not present.
    - Use Case            : When you need to find an element in a sorted container.

  3. **std::search**
    - Average Case        : O(n*m) (n = size of the container, m = size of the search sequence)
    - Best Case           : O(1)
    - Worst Case          : O(n*m)
    - Best Case Scenario  : The sequence is at the beginning of the container.
    - Worst Case Scenario : The sequence is at the end of the container or not present.
    - Use Case            : When you need to find a sequence of elements in a container.

  4. **std::search_n**
    - Average Case        : O(n)
    - Best Case           : O(1)
    - Worst Case          : O(n)
    - Best Case Scenario  : The sequence is at the beginning of the container.
    - Worst Case Scenario : The sequence is at the end of the container or not present.
    - Use Case            : When you need to find a sequence of repeated elements in a container.

  5. **std::adjacent_find**
    - Average Case        : O(n)
    - Best Case           : O(1)
    - Worst Case          : O(n)
    - Best Case Scenario  : The adjacent elements are at the beginning of the container.
    - Worst Case Scenario : The adjacent elements are at the end of the container or not present.
    - Use Case            : When you need to find two adjacent elements that satisfy a certain condition in a container.

  Remember, the choice of algorithm depends on the specific requirements of your program, such as whether the container is sorted, whether you're looking for a single element or a sequence, and so on.
  Always choose the algorithm that best fits your needs and provides the most efficient performance for your specific scenario.
