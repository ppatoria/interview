# Using doubly linked list (std::list):

```cpp
#include <iostream>
#include <list>

void reverseList(std::list<int>& l) {
    auto front = l.begin();        // Iterator pointing to the front
    auto back = l.rbegin();        // Reverse iterator pointing to the back

    // Continue swapping elements until the iterators meet or cross
    while (front != back.base()) {
        std::swap(*front, *back);  // Swap the elements
        ++front;                   // Move front iterator forward
        ++back;                    // Move back iterator backward
    }
}

int main() {
    std::list<int> myList = {1, 2, 3, 4, 5};
    
    std::cout << "Original list: ";
    for (int val : myList) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    reverseList(myList);

    std::cout << "Reversed list: ";
    for (int val : myList) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

---

### Explanation:
1. **Iterators for Both Ends**:
   - `front` starts from the beginning (`l.begin()`).
   - `back` starts from the end (`l.rbegin()`), which is a reverse iterator.
   - A reverse iterator’s `.base()` gives the corresponding forward iterator. This helps prevent overlapping when swapping elements.

2. **Swapping Elements**:
   - Use `std::swap` to exchange the values at the two iterators.

3. **Loop Until Iterators Meet or Cross**:
   - The loop stops when `front` reaches `back.base()` because reverse and forward iterators point to the same element at this point.

---

### Output Example:

```
Original list: 1 2 3 4 5 
Reversed list: 5 4 3 2 1
```

### Time and Space Complexity:
- **Time Complexity**: `O(n/2)` because we swap only half the elements.
- **Space Complexity**: `O(1)` since no extra space is used.

# Using singly linked list:
Singly linked lists in C++ do not have a built-in STL container (like `std::list` for doubly linked lists). To reverse a singly linked list, you would typically implement the logic yourself. Here's how you can approach the solution with a custom singly linked list implementation.

---

### Steps to Reverse a Singly Linked List
1. Use three pointers:
   - `prev`: Initially `nullptr`, tracks the previous node.
   - `current`: Points to the current node being processed.
   - `next`: Temporarily stores the next node to avoid losing track of the list.
2. Traverse the list:
   - At each step, reverse the `next` pointer of the `current` node to point to the `prev` node.
   - Move the `prev` and `current` pointers one step forward.
3. At the end of the traversal, the `prev` pointer will point to the new head of the reversed list.

---

### Code Implementation Using Singly Linked List
Here’s a simple implementation of reversing a singly linked list:

```cpp
#include <iostream>

struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
};

// Function to reverse a singly linked list
Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* current = head;
    Node* next = nullptr;

    while (current != nullptr) {
        next = current->next;  // Store the next node
        current->next = prev;  // Reverse the link
        prev = current;        // Move prev one step forward
        current = next;        // Move current one step forward
    }

    return prev;  // New head of the reversed list
}

// Helper function to print the list
void printList(Node* head) {
    Node* temp = head;
    while (temp != nullptr) {
        std::cout << temp->data << " ";
        temp = temp->next;
    }
    std::cout << "\n";
}

// Example usage
int main() {
    // Create a simple linked list: 1 -> 2 -> 3 -> 4 -> 5
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);
    head->next->next->next = new Node(4);
    head->next->next->next->next = new Node(5);

    std::cout << "Original List: ";
    printList(head);

    head = reverseList(head);

    std::cout << "Reversed List: ";
    printList(head);

    return 0;
}
```

---

### Explanation of the Code
1. **`reverseList` Function**:
   - The `while` loop iterates through the list.
   - Each node's `next` pointer is reversed to point to the previous node.
   - The traversal stops when the end of the list (`current == nullptr`) is reached.

2. **Time Complexity**:  
   - O(n), where `n` is the number of nodes in the list. Each node is visited once.

3. **Space Complexity**:  
   - O(1), as no extra memory is used apart from the three pointers (`prev`, `current`, and `next`).

---

# Using std::forward_list (which is a singly link list provided by stl)
Certainly! Below is a **non-recursive solution** to reverse a `std::forward_list` using an iterative approach. This approach is efficient, avoids recursion overhead, and works within the constraints of a singly linked list.

---

### Explanation:

1. **Key Idea**:
   - `std::forward_list` only allows forward traversal, so we must iteratively adjust the links to reverse the list.
   - Maintain three pointers: `prev`, `curr`, and `next`:
     - `prev` keeps track of the previous node.
     - `curr` is the current node being processed.
     - `next` stores the next node temporarily (since we lose access to it after changing `curr->next`).

2. **Algorithm**:
   - Start with `prev` as `nullptr` and `curr` pointing to the head of the list.
   - Traverse the list:
     - Save the next node.
     - Reverse the link of the current node to point to `prev`.
     - Move `prev` and `curr` one step forward.
   - Update the head of the list to `prev` (the new head after reversal).

---

### Implementation:

```cpp
#include <iostream>
#include <forward_list>

// Function to reverse a forward_list iteratively
void reverse(std::forward_list<int>& flist) {
    // Initialize pointers
    auto prev = flist.before_begin(); // Points to the "fake" node before the head
    auto curr = flist.begin();       // Start from the first element

    if (curr == flist.end() || std::next(curr) == flist.end()) {
        // List is empty or has only one element, nothing to reverse
        return;
    }

    auto next = std::next(curr); // Keep track of the next node

    while (next != flist.end()) {
        auto temp = std::next(next);    // Save the next node
        flist.splice_after(prev, flist, next); // Move the node after 'prev'
        next = temp;                    // Update 'next' to the saved node
    }
}

// Helper function to print a forward_list
void printList(const std::forward_list<int>& flist) {
    for (int value : flist) {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

// Example usage
int main() {
    std::forward_list<int> flist = {1, 2, 3, 4, 5};

    std::cout << "Original List: ";
    printList(flist);

    reverse(flist);

    std::cout << "Reversed List: ";
    printList(flist);

    return 0;
}
```

---

### Output:

```
Original List: 1 2 3 4 5 
Reversed List: 5 4 3 2 1 
```

---

### Time Complexity:
- **O(n)**: Each node is visited once and moved within the list.

### Space Complexity:
- **O(1)**: No additional memory is used beyond a few pointers.

---

### Why This Works:
- The use of `splice_after` allows efficient manipulation of the singly linked list, moving nodes without creating new ones.
- The head of the list is updated to `prev` after all nodes are processed, ensuring the list is reversed in place.

This method avoids recursion overhead and is optimal in both time and space for reversing a `std::forward_list`.
