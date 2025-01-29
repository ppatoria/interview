# Fixed Size Window

**Explanation of Fixed-Size Window:**

In the **fixed-size sliding window*** technique, a window of a predetermined size slides over a data structure (usually an array). At each step, you compute the result for the current window and adjust it as the window slides forward (by adding the new element entering the window and removing the one exiting).

- **Key idea:** Avoid recomputation by leveraging previous results as the window changes.
- **Use cases:** Problems like finding the maximum, minimum, or sum of elements in a sliding window.

---


## Find the Maximum in a Sliding Window
- **Question:** Given an array and an integer \( k \), find the maximum element in every contiguous subarray of size \( k \).  
  Example:  
  - Input: `nums = [1, 3, -1, -3, 5, 3, 6, 7]`, \( k = 3 \)  
  - Output: `[3, 3, 5, 5, 6, 7]`


Using `std::span` with a fixed window size sounds like a great idea for providing a lightweight, non-owning view into a segment of your array. However, `std::span` is simply a lightweight view of a contiguous sequence and doesn't directly provide functionalities like `min` or `max`. You can still leverage `std::span` to pass a window of the array to STL algorithms like `std::min_element` or `std::max_element`.


Here's a breakdown of why `std::span` can be helpful and its limitations in this context:

---

### **Why Use `std::span`?**
1. **Lightweight:** `std::span` doesn't copy the underlying data—it acts as a view, making it efficient.
2. **Simplicity:** You can iterate over a fixed-size window of the array easily without creating new containers.
3. **Compatibility:** Works well with STL algorithms like `std::min_element` and `std::max_element`.

---

### **Limitations**
1. **No Built-In Min/Max:** `std::span` does not have built-in `min` or `max` functions. You need to use external functions or STL algorithms like `std::min_element` and `std::max_element` to calculate the values.
2. **Inefficiency Without Optimization:** Calling `std::min_element` or `std::max_element` repeatedly for each window in a sliding window problem is inefficient (\(O(k)\) for each window), resulting in an overall \(O(n \times k)\) complexity, which is worse than \(O(n)\) solutions using a deque.

---

### **Example Using `std::span`**
Here's an example of using `std::span` for a sliding window maximum:

#### Code:
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <span>

std::vector<int> slidingWindowMaxWithSpan(const std::vector<int>& nums, int k) {
    std::vector<int> result;

    for (size_t i = 0; i <= nums.size() - k; ++i) {
        // Create a span representing the current window
        std::span<const int> window(nums.data() + i, k);

        // Find the max element in the current window
        int windowMax = *std::max_element(window.begin(), window.end());
        result.push_back(windowMax);
    }

    return result;
}

int main() {
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    std::vector<int> result = slidingWindowMaxWithSpan(nums, k);

    std::cout << "Max in Sliding Windows (Using std::span): ";
    for (int maxVal : result) {
        std::cout << maxVal << " ";
    }
    return 0;
}
```

---

### **Output**
For the input `nums = [1, 3, -1, -3, 5, 3, 6, 7]` and `k = 3`, the output is:  
```
Max in Sliding Windows (Using std::span): 3 3 5 5 6 7
```

---

### **Time and Space Complexity**
- **Time Complexity:** \(O(n \times k)\)  
  For every window, finding the max requires \(O(k)\), and there are \(n - k + 1\) windows.
- **Space Complexity:** \(O(1)\)  
  `std::span` does not allocate memory.

---

### **Why It's Not Optimal for Sliding Window Problems**
Although `std::span` is an elegant tool for representing a window, it doesn't provide the optimization needed for problems like sliding window maximum. Recalculating the max for each window makes it inefficient compared to solutions using a deque, which process the array in \(O(n)\).


## Smallest Subarray of Size 𝑘 k with Sum Greater Than 𝑋 X:
Return the smallest window of size 𝑘 k where the sum of elements is greater than a target value 𝑋 X.  
### Explanation

Let's break down the solution to find the smallest subarray of size \( k \) with a sum greater than \( X \) in simple steps:

#### Steps

1. **Initialize**:
   - Create variables to keep track of the current sum and the minimum length of the subarray.
   - Initialize pointers for the sliding window (`start` and `end`).

2. **Sliding Window**:
   - Slide the window from left to right, maintaining a sum of the current window of size \( k \).
   - If the sum of the current window is greater than \( X \), update the minimum length of the subarray if the current window is smaller than the previously found subarray.

3. **Shrink the Window**:
   - If the window size exceeds \( k \), shrink the window from the left (increment the `start` pointer and adjust the sum).

4. **Iterate**:
   - Continue this process until the `end` pointer reaches the end of the array.

5. **Result**:
   - Return the smallest window size found. If no such window exists, return an appropriate value (like -1).

### Walkthrough with Visualization

Let's take a sample array and target value:

- Array: `[1, 4, 45, 6, 10, 19]`
- \( k = 3 \)
- \( X = 50 \)

#### Initial Setup

- Current sum: `0`
- Minimum length: `infinity`
- Start: `0`

#### Step-by-Step Process

1. **Window: [1, 4, 45]**
   - Sum: `1 + 4 + 45 = 50` (equal to \( X \))
   - Window size: `3`
   - Since the sum is equal to \( X \), check the next window.

2. **Window: [4, 45, 6]**
   - Sum: `4 + 45 + 6 = 55` (greater than \( X \))
   - Window size: `3`
   - Update the minimum length to `3`.

3. **Window: [45, 6, 10]**
   - Sum: `45 + 6 + 10 = 61` (greater than \( X \))
   - Window size: `3`
   - The minimum length remains `3`.

4. **Window: [6, 10, 19]**
   - Sum: `6 + 10 + 19 = 35` (less than \( X \))
   - No update.

### Code Using STL and std

Here's the implementation in C++ using STL:

```cpp
#include <iostream>
#include <vector>
#include <climits>

int smallestSubarrayWithSumGreaterThanX(const std::vector<int>& arr, int k, int X) {
    int n = arr.size();
    int minLength = INT_MAX;
    int currentSum = 0;
    int start = 0;

    for (int end = 0; end < n; ++end) {
        currentSum += arr[end];

        if (end - start + 1 == k) {
            if (currentSum > X) {
                minLength = std::min(minLength, k);
            }
            currentSum -= arr[start];
            ++start;
        }
    }

    return (minLength == INT_MAX) ? -1 : minLength;
}

int main() {
    std::vector<int> arr = {1, 4, 45, 6, 10, 19};
    int k = 3;
    int X = 50;

    int result = smallestSubarrayWithSumGreaterThanX(arr, k, X);
    if (result != -1) {
        std::cout << "Smallest subarray length with sum greater than " << X << " is " << result << std::endl;
    } else {
        std::cout << "No subarray of size " << k << " has a sum greater than " << X << std::endl;
    }

    return 0;
}
```

### Time and Space Complexity

- **Time Complexity**: \( O(n) \)
  - We traverse the array only once using the sliding window technique, making the algorithm linear.

- **Space Complexity**: \( O(1) \)
  - We use only a fixed amount of extra space (a few variables), irrespective of the input size.

This approach ensures efficient processing with minimal overhead, providing an optimal solution for the given problem. If you have more questions or need further details, feel free to ask!
# Dynamic-size Window: 
Window grows/shrinks based on conditions (e.g., longest substring with at most \( k \) distinct characters).
### 1. Explanation of Dynamic-size Window

The **Dynamic-size Window** technique is a variation of the **Sliding Window** approach used to solve problems involving arrays or strings. Unlike the fixed-size sliding window, the dynamic-size window can grow or shrink based on certain conditions. This technique is particularly useful when you need to find a subarray or substring that satisfies a specific condition, such as having at most \( k \) distinct characters.

### 2. Common Interview Question

One common question asked in interviews is:

**"Find the length of the longest substring with at most \( k \) distinct characters."**

### 3. Walkthrough of the Solution

Let's break down the solution step-by-step in simple English:

1. **Initialize Pointers and Map**: Start with two pointers, `left` and `right`, both at the beginning of the string. Use a map (or dictionary) to keep track of the characters in the current window and their counts.

2. **Expand the Window**: Move the `right` pointer to the right, adding characters to the map. If the character is already in the map, increment its count. If it's not, add it to the map with a count of 1.

3. **Check the Condition**: After each expansion, check if the number of distinct characters in the map exceeds \( k \). If it does, you need to shrink the window from the left.

4. **Shrink the Window**: Move the `left` pointer to the right, removing characters from the map. Decrement the count of the character at the `left` pointer. If the count reaches zero, remove the character from the map.

5. **Update the Maximum Length**: After each valid window (i.e., when the number of distinct characters is at most \( k \)), update the maximum length of the substring found so far.

6. **Repeat**: Continue the process until the `right` pointer reaches the end of the string.

**Visualization**:

Consider the string `"aabbcc"` and \( k = 2 \):

- Start with `left = 0`, `right = 0`, map = `{}`, max_length = `0`.
- Move `right` to `0`: map = `{'a': 1}`, max_length = `1`.
- Move `right` to `1`: map = `{'a': 2}`, max_length = `2`.
- Move `right` to `2`: map = `{'a': 2, 'b': 1}`, max_length = `3`.
- Move `right` to `3`: map = `{'a': 2, 'b': 2}`, max_length = `4`.
- Move `right` to `4`: map = `{'a': 2, 'b': 2, 'c': 1}` (now distinct characters > \( k \)), so move `left` to `1`: map = `{'a': 1, 'b': 2, 'c': 1}`.
- Move `right` to `5`: map = `{'a': 1, 'b': 2, 'c': 2}` (still > \( k \)), so move `left` to `2`: map = `{'b': 2, 'c': 2}`, max_length = `4`.

The longest substring with at most 2 distinct characters is `"aabb"` or `"bbcc"`, both of length 4.

### 4. Code Using STL and std

```cpp
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

int longestSubstringWithKDistinct(const std::string& s, int k) {
    if (k == 0) return 0;

    std::unordered_map<char, int> charCount;
    int left = 0;
    int max_length = 0;

    for (int right = 0; right < s.size(); ++right) {
        charCount[s[right]]++;

        while (charCount.size() > k) {
            charCount[s[left]]--;
            if (charCount[s[left]] == 0) {
                charCount.erase(s[left]);
            }
            left++;
        }

        max_length = std::max(max_length, right - left + 1);
    }

    return max_length;
}

int main() {
    std::string s = "aabbcc";
    int k = 2;
    std::cout << "Length of the longest substring with at most " << k
              << " distinct characters: " << longestSubstringWithKDistinct(s, k) << std::endl;
    return 0;
}
```

### 5. Time and Space Complexity

- **Time Complexity**: \( O(n) \), where \( n \) is the length of the string. Each character is processed at most twice (once by the `right` pointer and once by the `left` pointer).
- **Space Complexity**: \( O(k) \), where \( k \) is the number of distinct characters. In the worst case, the map will store at most \( k + 1 \) characters.

This solution efficiently finds the longest substring with at most \( k \) distinct characters using the dynamic-size window technique.

# Two-pointer Technique:
Start with two pointers to define the window and adjust them independently (e.g., find subarray with a given sum).
### 1. Explanation
The two-pointer technique is a powerful method used to solve a variety of problems that involve searching for subarrays or subsequences in arrays or strings. It typically involves two pointers that move through the data structure, independently or in tandem, to find a solution. This technique is particularly useful for problems with constraints on the sum, length, or content of subarrays or subsequences.

### 2. Common Interview Question
One common interview question using the two-pointer technique is: "Find a continuous subarray in a given array that adds up to a given sum."

### 3. Solution Walkthrough (Simple English with Visualization)
Imagine you have a string of beads, each bead representing a number. Your goal is to find a segment of beads that adds up to a specific total.

1. Start with two pointers, both at the beginning of the string.
2. One pointer (let’s call it `start`) marks the beginning of the segment, while the other pointer (let’s call it `end`) marks the end of the segment.
3. Expand the segment by moving the `end` pointer to the right, adding the value of the new bead to a running total.
4. If the running total exceeds the target sum, move the `start` pointer to the right to shrink the segment until the total is less than or equal to the target sum.
5. Continue adjusting the pointers until you find a segment that matches the target sum.

**Visualization Example:**

Let's say the array is `[1, 2, 3, 7, 5]` and the target sum is `12`.

- Start with both pointers at index 0.
- Expand the segment: `[1]` (sum = 1).
- Expand the segment: `[1, 2]` (sum = 3).
- Expand the segment: `[1, 2, 3]` (sum = 6).
- Expand the segment: `[1, 2, 3, 7]` (sum = 13, exceeds target).
- Shrink the segment: `[2, 3, 7]` (sum = 12, matches target).

The segment `[2, 3, 7]` is the subarray that adds up to the target sum of 12.

### 4. Code Using STL and std
Here's an example in C++ using the STL:

```cpp
#include <iostream>
#include <vector>
using namespace std;

vector<int> subarrayWithSum(const vector<int> &arr, int target) {
    int start = 0, end = 0, sum = 0;
    while (end < arr.size()) {
        sum += arr[end];
        while (sum > target && start <= end) {
            sum -= arr[start];
            start++;
        }
        if (sum == target) {
            return vector<int>(arr.begin() + start, arr.begin() + end + 1);
        }
        end++;
    }
    return {}; // Return empty vector if no subarray found
}

int main() {
    vector<int> arr = {1, 2, 3, 7, 5};
    int target = 12;
    vector<int> result = subarrayWithSum(arr, target);

    if (!result.empty()) {
        cout << "Subarray with the given sum: ";
        for (int num : result) {
            cout << num << " ";
        }
    } else {
        cout << "No subarray with the given sum found.";
    }
    return 0;
}
```

### 5. Time and Space Complexity
- **Time Complexity:** \( O(n) \), where \( n \) is the length of the array. Each element is processed at most twice (once by the `end` pointer and once by the `start` pointer).
- **Space Complexity:** \( O(1) \), since we are not using any extra space proportional to the input size, except for the output.



# Cyclic Window:
Handle circular arrays by "wrapping" the window (e.g., maximum sum in a circular subarray).

### (1) Explanation: Cyclic Window

A **cyclic window** is used to solve problems involving circular arrays, where the array is treated as if it wraps around from the last element back to the first. This is useful for scenarios like finding the maximum sum of a subarray in a circular array. 

For example:
- In a circular array `[1, 2, 3, 4]`, a subarray can include elements like `[3, 4, 1]`, "wrapping around" from the end to the start.

The key challenge with cyclic arrays is handling the wrap-around efficiently without physically duplicating the array.

---

### (2) Selected Problem: Maximum Sum in a Circular Subarray

**Problem Statement:**  
Given a circular array of integers, find the maximum sum of a subarray in the array.

**Examples:**
1. **Input:** `[5, -3, 5]`  
   **Output:** `10` (Maximum sum is achieved by the subarray `[5, 5]`, wrapping around the array).

2. **Input:** `[-3, -2, -1]`  
   **Output:** `-1` (Maximum sum is `-1`, as all numbers are negative).

---

### (3) Walkthrough with Visualization (Simple English)

The solution can be divided into two parts:

#### Step 1: Maximum Sum without Wrap-Around
- Use **Kadane's Algorithm** to find the maximum sum of a subarray in a **linear array**.
- Example: For `[5, -3, 5]`, Kadane’s algorithm gives `7` (`[5, -3, 5]`).

#### Step 2: Maximum Sum with Wrap-Around
- To account for wrap-around:
  1. Compute the total sum of the array (`totalSum`).
  2. Find the **minimum subarray sum** (using a modified Kadane's algorithm).
  3. The maximum sum with wrap-around is:  
     `totalSum - minSubarraySum`.

#### Edge Case:
- If all elements are negative, the result is the maximum single element (Kadane's result).

#### Final Answer:
The result is the **maximum** of:
- Maximum sum without wrap-around (from Step 1).
- Maximum sum with wrap-around (from Step 2).

---

### Visualization

**Input:** `[5, -3, 5]`

1. **Kadane's Algorithm for Maximum Subarray (Without Wrap-Around):**  
   - Current sum: `[5] → [5 + (-3)] = 2 → [2 + 5] = 7`.  
   - Maximum sum = `7`.

2. **Compute Total Sum and Minimum Subarray:**  
   - Total sum = `5 + (-3) + 5 = 7`.  
   - Minimum subarray sum:  
     Current sum: `[5] → [5 + (-3)] = 2 → [2 + 5] = 7`.  
     Minimum sum = `-3`.

3. **Wrap-Around Sum:**  
   - Wrap-around max sum = `totalSum - minSubarraySum = 7 - (-3) = 10`.

4. **Final Answer:**  
   - Maximum without wrap-around = `7`.  
   - Maximum with wrap-around = `10`.  
   **Result = 10.**

---

### (4) Code Using STL and `std`

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// Helper function to calculate maximum subarray sum using Kadane's algorithm
int kadaneMax(const vector<int>& nums) {
    int currentSum = 0, maxSum = nums[0];
    for (int num : nums) {
        currentSum = max(num, currentSum + num);
        maxSum = max(maxSum, currentSum);
    }
    return maxSum;
}

// Helper function to calculate minimum subarray sum using Kadane's algorithm
int kadaneMin(const vector<int>& nums) {
    int currentSum = 0, minSum = nums[0];
    for (int num : nums) {
        currentSum = min(num, currentSum + num);
        minSum = min(minSum, currentSum);
    }
    return minSum;
}

int maxSumCircularSubarray(const vector<int>& nums) {
    int totalSum = accumulate(nums.begin(), nums.end(), 0); // Total sum of the array

    // Step 1: Maximum subarray sum without wrap-around
    int maxNoWrap = kadaneMax(nums);

    // Step 2: Minimum subarray sum for wrap-around calculation
    int minSubarraySum = kadaneMin(nums);

    // Maximum subarray sum with wrap-around
    int maxWithWrap = totalSum - minSubarraySum;

    // Handle edge case: if all numbers are negative
    if (maxWithWrap == 0) {
        return maxNoWrap;
    }

    // Return the maximum of the two
    return max(maxNoWrap, maxWithWrap);
}

int main() {
    vector<int> nums = {5, -3, 5};

    cout << "Maximum sum in circular subarray: "
         << maxSumCircularSubarray(nums) << endl;

    return 0;
}
```

---

### (5) Time and Space Complexity

- **Time Complexity:** \( O(n) \)
  - Each Kadane's algorithm call (for max and min subarray sums) takes \( O(n) \).
  - Summing the array using `accumulate` also takes \( O(n) \).
  - Overall: \( O(n) \).

- **Space Complexity:** \( O(1) \)
  - Only a few variables are used; no additional data structures are created.

---

### Output for Input:

**Array:** `[5, -3, 5]`  
**Result:** `10`  

For another input:  
**Array:** `[-3, -2, -1]`  
**Result:** `-1`.
# Overlapping Windows:
Process overlapping segments of data (e.g., repeated DNA sequences). 
### 1. Explanation
Overlapping windows are used to process segments of data that can overlap with each other. This technique is beneficial for problems where the segments can share common elements, such as finding repeated sequences in DNA, where some subsequences might overlap.

### 2. Common Interview Question
One common interview question involving overlapping windows is: "Find all the 10-letter-long sequences (substrings) that occur more than once in a DNA molecule."

### 3. Solution Walkthrough (Simple English with Visualization)
Imagine you have a long piece of string, each segment of the string is made up of letters representing DNA sequences. You want to find all the segments of a certain length that appear more than once in this string. 

1. Slide a window of fixed length (e.g., 10 characters) over the string.
2. At each position, record the current segment of the string.
3. Check if the current segment has appeared before:
   - If yes, add it to the list of repeated sequences.
   - If no, store it in a data structure to keep track.
4. Move the window one position to the right and repeat until you have processed the entire string.

**Visualization Example:**

Let's say the DNA sequence is `AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT` and we are looking for 10-letter sequences.

- Start with the window at the beginning: `AAAAACCCCC`.
- Move the window one position to the right: `AAAACCCCCA`.
- Continue sliding the window, recording and checking segments: `AAACCCCCAA`, `AACCCCCAAA`, and so on.
- If a segment appears more than once, like `AAAAACCCCC`, add it to the list of repeated sequences.

### 4. Code Using STL and std
Here's an example in C++ using the STL:

```cpp
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

vector<string> findRepeatedDnaSequences(const string &s) {
    unordered_set<string> seen, repeated;
    vector<string> result;

    for (size_t i = 0; i + 9 < s.size(); ++i) {
        string sequence = s.substr(i, 10);
        if (seen.find(sequence) != seen.end()) {
            repeated.insert(sequence);
        } else {
            seen.insert(sequence);
        }
    }

    for (const string &seq : repeated) {
        result.push_back(seq);
    }
    return result;
}

int main() {
    string dna = "AAAAACCCCCAAAAACCCCCCAAAAAGGGTTT";
    vector<string> result = findRepeatedDnaSequences(dna);

    if (!result.empty()) {
        cout << "Repeated DNA sequences: ";
        for (const string &seq : result) {
            cout << seq << " ";
        }
    } else {
        cout << "No repeated DNA sequences found.";
    }
    return 0;
}
```

### 5. Time and Space Complexity
- **Time Complexity:** \( O(n) \), where \( n \) is the length of the DNA sequence. Each 10-letter sequence is processed in constant time.
- **Space Complexity:** \( O(n) \), due to the space required to store seen and repeated sequences.

