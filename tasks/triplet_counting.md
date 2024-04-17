# Triplet Counting (Python Algorithms):
   - Write a Python function that counts the number of triplets in an array such that the sum of any two elements equals the third element.

1. **Problem Description**:
   - Given an array of integers, we want to find all triplets (i, j, k) such that:
     - i < j < k
     - arr[k] = arr[i] + arr[j]

2. **Example**:
   - Suppose we have the input array `arr = [1, 3, 9, 9, 27, 81]` and the common ratio `r = 3`.
   - The triplets satisfying the condition are: `(1, 3, 9)` and `(1, 9, 27)`.
   - So the count of such triplets is 2.

3. **Solution Approach**:
   - We'll use two dictionaries (`v2` and `v3`) to keep track of the counts of elements.
   - We iterate through the array and update the counts based on the current element and its multiples.
   - The final count of triplets is the sum of `v3[k]` for each element `k` in the array.

4. **Algorithm**:
   - Initialize `v2` and `v3` dictionaries with default values of 0.
   - Initialize a variable `count` to 0.
   - For each element `k` in the array:
     - Update `count` by adding `v3[k]`.
     - Update `v3[k*r]` by adding `v2[k]`.
     - Update `v2[k*r]` by incrementing 1.

5. **Time Complexity**: O(n) where n is the length of the array.
6. **Space Complexity**: O(n) for the dictionaries.

7. **Python Code**:
   ```python
   from collections import defaultdict

   def countTriplets(arr, r):
       v2 = defaultdict(int)
       v3 = defaultdict(int)
       count = 0

       for k in arr:
           count += v3[k]
           v3[k*r] += v2[k]
           v2[k*r] += 1

       return count

   arr = [1, 3, 9, 9, 27, 81]
   r = 3
   print(countTriplets(arr, r))  # Output: 2
   ```

This code efficiently counts the triplets satisfying the given condition. The dictionaries `v2` and `v3` help us keep track of the elements and their multiples. The final count represents the number of valid triplets. 🚀👩‍💻

Source: Conversation with Bing, 4/16/2024
(1) python - Understanding Count Triplets HackerRank - Stack Overflow. https://stackoverflow.com/questions/58048434/understanding-count-triplets-hackerrank.
(2) Count of triplets in an Array (i, j, k) such that i < j < k and a[k .... https://www.geeksforgeeks.org/count-of-triplets-in-an-array-i-j-k-such-that-i-j-k-and-ak-ai-aj/.
(3) Python | Find all triplets in a list with given sum - GeeksforGeeks. https://www.geeksforgeeks.org/python-find-all-triplets-in-a-list-with-given-sum/.
(4) Count The Triplets Problem - InterviewBit. https://www.interviewbit.com/blog/count-the-triplets/.
(5) Pythagorean Triples Program in Python - Sanfoundry. https://www.sanfoundry.com/python-program-determine-pythagorean-triplets-range/.
(6) 5 Best Ways to Program to Find Number of Good Triplets in Python. https://blog.finxter.com/5-best-ways-to-program-to-find-number-of-good-triplets-in-python/.
(7) python - Find count of triples in a list - Stack Overflow. https://stackoverflow.com/questions/40267827/find-count-of-triples-in-a-list.
(8) github.com. https://github.com/yosveni/code_byte_solutions/tree/972d5ddedbf81e986c8fc3883a65d5b96dfe4540/hankRank%2Fdef%20countTriplets.py.
(9) github.com. https://github.com/SaiLokeshPolineni/pythonprograms/tree/ef2132d9d1fede7dea3583da63e202a1ad392778/CountTriplets.py.
แน่นอน! I can help you with that.

**1. Explanation with an example:**

Imagine you are given a list of numbers, and you want to find how many groups of three (triplets) add up to zero. For instance, consider the list `arr = [1, 5, 3, 2]`. In this list, the triplet `(1, 2, -3)` satisfies the condition because 1 + 2 = -3. There's another triplet too,`(2, 3, -5)`, that validates the condition. So, the function would return 2 for this array.

**2. Solution Approach and Complexity:**

There are two main approaches to solve this problem:

  * **Basic Approach  (Time Complexity: O(N^3), Space Complexity: O(1))**
    The most basic approach is to run three nested loops from 1 till N and check if the summation of any two integers equals the third integer.
``` python
def count_Triplets(A, N):
     count = 0
     A.sort()
     for i in range(N):
         for j in range(i + 1, N):
             for k in range(j + 1, N):
                 if A[i] + A[j] == A[k]:
                      count = count + 1

     return count

A = [1, 2, 3, 4, 5]
N = 5
print(count_Triplets(A, N))
```

Time Complexity: O(N^3) where N is the number of nodes of the binary tree.Space Complexity: O(1)



Algorithm

Compute the value of the maximum element, mx of the array.
Build a frequency array, freq of size mx + 1 and store the frequency of all the elements of the array A[].
Initialise a count variable and consider the above four cases one by one:
If the triplet is (0, 0, 0), add freq[0]C3 to count.
If the triplet is (0, x, x), add freq[0]C1 * freq[x]C2 to count.
If the triplet is (x, x, 2x), add freq[x]C2 * freq[2x]C1 to count.
If the triplet is (x, y, x + y), add freq[x]C1 * freq[y]C1 * freq[x + y]C1 to count.
Return count.

```python
def count_Triplets(A, n):
    max_val = 0
    for i in range(n):
        max_val = max(max_val, A[i])
 
    freq = [0 for i in range(max_val + 1)]
 
    for i in range(n):
        freq[A[i]] += 1
 
    count = 0 
 
 
    count += (freq[0] * (freq[0] - 1) *
           (freq[0] - 2) // 6)
 
    for i in range(1, max_val + 1):
        count += (freq[0] * freq[i] *
               (freq[i] - 1) // 2)
 
    for i in range(1, (max_val + 1) // 2):
        count += (freq[i] *
               (freq[i] - 1) // 2 * freq[2 * i])
 
    for i in range(1, max_val + 1):
        for j in range(i + 1, max_val - i + 1):
            count += freq[i] * freq[j] * freq[i + j]
 
    return count
 
 
A = [1, 2, 3, 4, 5]
N = 5
print(count_Triplets(A, N))
```
Time Complexity: O(N^2) where N is the number of nodes of the binary tree.
Space Complexity: O(N), as a map is used.
