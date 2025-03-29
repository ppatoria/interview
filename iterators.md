# iterator design pattern
```c++
#include <iostream>

// Step 1: Custom Container (STL-like) Using C-Style Array
template <typename T, size_t Size>
class CustomArray {
private:
    T data[Size];  // Fixed-size array
    size_t count;  // Tracks number of elements added

public:
    CustomArray() : count(0) {}

    void add(const T& value) {
        if (count < Size) {
            data[count++] = value;
        } else {
            std::cerr << "Container is full!" << std::endl;
        }
    }

    size_t size() const {
        return count;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    // Forward declaration of iterator
    class Iterator;

    // STL-like begin() and end() functions
    Iterator begin() { return Iterator(*this, 0); }
    Iterator end() { return Iterator(*this, count); }

    // Step 2: Custom Iterator Class Inside Container
    class Iterator {
    private:
        CustomArray& container;
        size_t index;

    public:
        Iterator(CustomArray& c, size_t idx) : container(c), index(idx) {}

        bool operator!=(const Iterator& other) const {
            return index != other.index;
        }

        T& operator*() {
            return container.data[index];
        }

        Iterator& operator++() {
            ++index;
            return *this;
        }
    };
};

// Step 3: Client Code
int main() {
    CustomArray<int, 5> myArray;

    myArray.add(10);
    myArray.add(20);
    myArray.add(30);

    std::cout << "Iterating over CustomArray: ";
    for (auto it = myArray.begin(); it != myArray.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
```
# iterator invalidation:
Iterator invalidation is a common issue in the C++ standard library.
It occurs when an iterator no longer points to a valid element in a container, or when the state of the container changes in a way that makes the iterator invalid.

For example, if you erase an element from a vector, all iterators and references after the point of erase are invalidated.
Similarly, if you insert an element into a deque, all iterators and references are invalidated, unless the inserted member is at an end (front or back) of the deque.

To avoid iterator invalidation, it's important to be aware of the rules for each container and to use the appropriate container member functions.
In general, invoking a container member function or passing a container as an argument to a library function shall not invalidate iterators to, or change the values of, objects within that container, unless otherwise specified.

## The possible scenarios of iterator invalidation for various containers:

### Insertion Sequence containers:
    * `vector`: All iterators and references before the point of insertion are unaffected, unless the new container size is greater than the previous capacity (in which case all iterators and references are invalidated).
    * `deque`: All iterators and references are invalidated, unless the inserted member is at an end (front or back) of the deque (in which case all iterators are invalidated, but references to elements are unaffected).
    * `list`: All iterators and references are unaffected.
- Associative containers [multi] {set,map}: All iterators and references are unaffected.
- Container adaptors:
    * `stack`: Inherited from underlying container.
    * `queue`: Inherited from underlying container.
    * `priority_queue`: Inherited from underlying container.
- Erasure Sequence containers:
    * `vector`: Every iterator and reference after the point of erase is invalidated.
    * `deque`: All iterators and references are invalidated, unless the erased members are at an end (front or back) of the deque (in which case only iterators and references to the erased members are invalidated).
    * `list`: Only the iterators and references to the erased element is invalidated.
- Resizing:
    * `vector`: As per insert/erase.
    * `deque`: As per insert/erase.
    * `list`: As per insert/erase.

Please note that unless otherwise specified, invoking a container member function or passing a container as an argument to a library function shall not invalidate iterators to, or change the values of, objects within that container ¹.

I hope this helps! Let me know if you have any other questions.

Source: Conversation with Bing, 3/7/2024
(1) Iterator invalidation rules for C++ containers - Stack Overflow. https://stackoverflow.com/questions/6438086/iterator-invalidation-rules-for-c-containers.
(2) Safe Iterator Framework for the C++ Standard Template Library. https://pdfs.semanticscholar.org/771f/8cf8c3cc90c8cee2f27764e3aba51d761f33.pdf.
(3) Do the iterator invalidation rules mean thread safety?. https://stackoverflow.com/questions/32017363/do-the-iterator-invalidation-rules-mean-thread-safety.
(4) c++ - What is iterator invalidation? - Stack Overflow. https://stackoverflow.com/questions/16904454/what-is-iterator-invalidation.
(5) Iterator library - cppreference.com. https://en.cppreference.com/w/cpp/iterator.


Invalidation of iterators in the C++ Standard Library refers to situations where an iterator becomes unusable because the underlying container it references has been modified in a way that breaks the association between the iterator and the element it pointed to. Here are the common scenarios of iterator invalidation in the standard library containers:


**1. Modification Operations:**

- **Insertion:** Inserting elements before the iterator (using `insert` or similar methods) invalidates all iterators at or after the insertion point.
- **Removal:** Removing elements pointed to by the iterator or elements before it invalidates the iterator.
- **Swapping:** Swapping elements within the container might invalidate iterators pointing to the swapped elements, depending on the implementation.
- **`clear`:** Clearing the container invalidates all iterators.

**2. Resizing Operations:**

- **`resize`:** Resizing the container (changing its size) can invalidate all iterators, especially if the container needs to reallocate memory.

**3. Specific Container Behaviors:**

- **`std::vector`:** Iterators become invalid after any operation that modifies the vector's capacity (potentially reallocating memory), such as `push_back` when the vector is full, or `erase` followed by insertions that might cause reallocation.
- **`std::deque`:** Iterators might become invalid after insertions or removals near the beginning or end of the deque, as these operations might involve reallocation.
- **`std::list`:** Iterators remain valid after most operations, except for `splice` when it moves elements within the same list and the iterator points to a moved element.

**Important Notes:**

- **Past-the-end iterators:** These iterators (returned by `end()`) are generally not invalidated by modifications to the container, but their behavior might vary slightly depending on the specific container and operations performed.
- **Const member functions:** Iterators returned by const member functions (e.g., `cbegin()`, `cend()`) are typically guaranteed not to be invalidated by const member functions that don't modify the container.

**Recommendations:**

- **Re-obtain iterators:** If you're unsure about the validity of an iterator after an operation, it's generally safe to re-obtain the iterator from the container (e.g., using `begin()`) to ensure it points to a valid element.
- **Const iterators:** Use const iterators whenever possible when you only need to read elements without modifying the container. These iterators are less likely to be invalidated.
- **Understand container behavior:** Be familiar with the specific invalidation rules for the container you're using. Consult the C++ documentation for details.

By understanding these scenarios of iterator invalidation, you can write more robust and predictable C++ code that avoids unexpected behavior when iterating through containers. 



## Range-based for loops (range-for) and Iterator Invalidation

**Yes, range-based for loops are generally considered to be constant-time iterations.** This means the number of iterations doesn't directly affect the performance of the loop itself. However, it's important to be aware of iterator invalidation within the loop body, which can cause unexpected behavior.

Here's a breakdown of iterator invalidation for common containers and how to prevent it:

**1. `std::vector`:**

- **Scenarios:**
    - Inserting elements before the iterator (using `insert`) invalidates all iterators at or after the insertion point.
    - Removing elements pointed to by the iterator or elements before it invalidates the iterator.
    - Swapping elements within the vector might invalidate iterators pointing to the swapped elements if the swap involves reallocating memory (potential with full vector).
    - Resizing the vector (`resize`) can invalidate all iterators due to potential reallocation.
- **Prevention:**
    - Avoid modifying the vector's size or elements within the loop body if you rely on the iterator remaining valid.
    - If you need to modify the vector, consider using a different loop construct that allows you to control iterator invalidation (e.g., traditional for loop with index).

**2. `std::deque`:**

- **Scenarios:**
    - Inserting or removing elements near the beginning or end of the deque might invalidate iterators, especially for large insertions/removals, as they might trigger reallocation.
- **Prevention:**
    - Similar to `std::vector`, avoid modifying the deque's size or elements near the beginning or end within the loop if you rely on the iterator.
    - Consider using a different loop construct or techniques like iterators pointing to specific elements (e.g., by index) if needed.

**3. `std::list`:**

- **Scenarios:**
    - `splice` when it moves elements within the same list and the iterator points to a moved element.
- **Prevention:**
    - If using `splice` within the loop, be cautious about iterators pointing to elements that might be moved.
    - Consider re-obtaining iterators from the list after `splice` if necessary.

**4. `std::set` and `std::map`:**

- **Scenarios:**
    - Inserting or removing elements generally don't invalidate iterators unless the element being removed is the one pointed to by the iterator.
- **Prevention:**
    - Modifying elements within the loop (which isn't typical for sets/maps) might invalidate iterators in some cases. It's generally recommended to avoid modifying elements within the loop for these containers.

**Example of Swapping Leading to Invalidation:**

```c++
#include <iostream>
#include <vector>

int main() {
  std::vector<int> vec = {1, 2, 3, 4};
  std::vector<int>::iterator it = vec.begin(); // Iterator pointing to the first element (1)

  // Loop through the vector using range-for
  for (int& elem : vec) {
    elem *= 2; // Double each element
    if (elem == 4) { // Check if current element is 4
      std::swap(elem, vec[2]); // Swap element with element at index 2
    }
  }

  // Try to access the first element after the loop (assuming it was 2 before)
  std::cout << *it << std::endl; // This might print unexpected value or cause undefined behavior

  return 0;
}
```

In this example:

- The loop iterates and doubles each element.
- If an element is 4, it's swapped with the element at index 2.
- However, swapping elements within the vector might involve reallocation if the vector is full. This can invalidate the iterator `it` as it might no longer point to the originally intended element.

**Prevention:**

- Avoid modifying the vector's structure (size or element order) within the range-based for loop if you rely on the iterator's validity throughout the loop.
- If you need to swap elements, consider using a different approach like a traditional for loop with index-based access or a separate loop for modifications.

**General Tips:**

- Be cautious about modifying the container within a range-based for loop, especially if you rely on iterator validity.
- If unsure about potential invalidation, re-obtain iterators from the container or use a different loop construct (e.g., traditional for loop with index) that allows more control over iterators.
- Understand the specific invalidation rules for the container you're using



# A container providing iterators is functionally more flexible than one providing "void visit(auto functor_to_call_for_each_node)" instead
Yes, Iterators provide more flexibility as they allow the user of the container to control the traversal, whereas a visit function encapsulates the traversal within the container.
let's consider a simple example of a list of integers and we want to find a specific value in the list.
If we have an iterator, we can control the traversal and stop when we find the value. Here is a C++ example:

```cpp
std::list<int> myList = {1, 2, 3, 4, 5};
int valueToFind = 3;
for(auto it = myList.begin(); it != myList.end(); ++it) {
    if(*it == valueToFind) {
        std::cout << "Found " << valueToFind << std::endl;
        break;
    }
}
```

In this case, we have full control over the traversal.
We can decide when to start, when to stop, and what to do at each step.

On the other hand, if we only have a `visit` function, we can't control the traversal in the same way.
The `visit` function would look something like this:

```cpp
void visit(std::list<int>& myList, std::function<void(int)> functor) {
    for(const auto& value : myList) {
        functor(value);
    }
}
```

And we would use it like this:

```cpp
visit(myList, [](int value) {
    if(value == valueToFind) {
        std::cout << "Found " << valueToFind << std::endl;
        // But we can't stop the traversal here!
    }
});
```

As you can see, with the `visit` function, we don't have the same level of control.
We can't stop the traversal when we find the value.
The `visit` function will always go through every element in the list.

This is a simple example, but it illustrates the point.
Iterators provide more flexibility because they give the user more control over the traversal.
This can be very useful in more complex scenarios where the user might want to control the traversal in ways that the `visit` function doesn't allow.
For example, the user might want to skip certain elements, or stop the traversal under certain conditions, or traverse the elements in a non-linear order.
These things are not possible with a `visit` function.
So while a `visit` function can be simpler and easier to use in some cases, an iterator is generally more flexible.
