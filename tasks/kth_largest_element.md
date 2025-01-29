### 1. **Approach 1: Max-Heap with K Removals**  

#### Explanation:
1. Create a **max-heap** of all elements in the array. In C++, a priority queue is a max-heap by default.
2. Remove the largest element (`k-1` times) from the heap.
3. The Kth largest element will be the top of the heap.

#### Time Complexity:
- **Building the Max-Heap:** \(O(n)\).
- **Removing \(k-1\) elements:** Each removal is \(O(\log n)\), so total \(O(k \log n)\).
- **Overall:** \(O(n + k \log n)\).

#### Space Complexity:
- \(O(n)\) for storing the heap.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int findKthLargest_MaxHeap(vector<int>& nums, int k) {
    // Create a max-heap using a priority queue
    priority_queue<int> maxHeap(nums.begin(), nums.end());
    
    // Remove the top (k-1) elements
    for (int i = 0; i < k - 1; ++i) {
        maxHeap.pop();
    }

    // The top element is the Kth largest
    return maxHeap.top();
}

int main() {
    vector<int> nums = {3, 2, 1, 5, 6, 4};
    int k = 2;

    cout << "The " << k << "th largest element is: "
         << findKthLargest_MaxHeap(nums, k) << endl;
    return 0;
}
```

#### Example Walkthrough:
For `nums = {3, 2, 1, 5, 6, 4}`, `k = 2`:
1. Build max-heap: `[6, 5, 4, 3, 2, 1]`.
2. Remove the largest: `6`.
3. Remove the next largest: `5`.
4. Top element of the heap is `4`, which is the 2nd largest.

---

### 2. **Approach 2: Min-Heap of Size K**

#### Explanation:
1. Use a **min-heap** of size \(k\). In C++, this is done using a priority queue with a custom comparator.
2. Traverse the array:
   - Add each element to the heap.
   - If the heap size exceeds \(k\), remove the smallest element (root).
   - The heap will maintain the \(k\) largest elements in the array.
3. At the end, the root of the heap is the Kth largest element.

#### Time Complexity:
- **Building the Heap:** Inserting an element in the heap takes \(O(\log k)\).
- **Processing \(n\) elements:** \(O(n \log k)\).
- **Overall:** \(O(n \log k)\).

#### Space Complexity:
- \(O(k)\) for storing the heap.

#### C++ Code:
```cpp
#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int findKthLargest_MinHeap(vector<int>& nums, int k) {
    // Create a min-heap of size k
    priority_queue<int, vector<int>, greater<int>> minHeap;

    for (int num : nums) {
        minHeap.push(num);
        // Maintain the size of the heap to be at most k
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    // The root of the heap is the Kth largest element
    return minHeap.top();
}

int main() {
    vector<int> nums = {3, 2, 1, 5, 6, 4};
    int k = 2;

    cout << "The " << k << "th largest element is: "
         << findKthLargest_MinHeap(nums, k) << endl;
    return 0;
}
```

#### Example Walkthrough:
For `nums = {3, 2, 1, 5, 6, 4}`, `k = 2`:
1. Start with an empty min-heap.
2. Add `3`: Heap becomes `[3]`.
3. Add `2`: Heap becomes `[2, 3]`.
4. Add `1`: Heap becomes `[1, 3, 2]` → Pop smallest: Heap becomes `[2, 3]`.
5. Add `5`: Heap becomes `[2, 3, 5]` → Pop smallest: Heap becomes `[3, 5]`.
6. Add `6`: Heap becomes `[3, 5, 6]` → Pop smallest: Heap becomes `[5, 6]`.
7. Add `4`: Heap becomes `[4, 6, 5]` → Pop smallest: Heap becomes `[5, 6]`.
8. Root is `5`, which is the 2nd largest.

---

### Comparison of Approaches

| Approach                | Time Complexity  | Space Complexity | Notes                                                  |
|-------------------------|------------------|------------------|-------------------------------------------------------|
| Max-Heap with K Removals | \(O(n + k \log n)\) | \(O(n)\)         | Simple to implement but less efficient for large \(k\). |
| Min-Heap of Size K       | \(O(n \log k)\)   | \(O(k)\)         | More efficient for large \(k\) and large arrays.       |

---
