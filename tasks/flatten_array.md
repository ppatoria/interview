# Flatten N-Dimensional Array to 1D Array:
   - Implement a function that takes an N-dimensional array (nested lists) and returns a flattened 1D array.

The `flatten_ndarray` function recursively flattens an N-dimensional array (nested lists) into a 1D array. Let's break down the example usage and the algorithm:

1. **Example Usage**:
   - The input array is `arr = [[1, 2, 3], [4, [5, 6]], 7]`.
   - The expected flattened 1D array is `[1, 2, 3, 4, 5, 6, 7]`.

2. **Algorithm**:
   - Initialize an empty list called `result`.
   - For each element `elem` in the input array:
     - If `elem` is a list, recursively flatten it by calling `flatten_ndarray(elem)`.
     - Otherwise, append `elem` to the `result` list.
   - Return the final `result`.

3. **Explanation**:
   - The function processes each element in the nested array:
     - `[1, 2, 3]` is appended to `result`.
     - `[4, [5, 6]]` is recursively flattened:
       - `4` is appended to `result`.
       - `[5, 6]` is recursively flattened:
         - `5` and `6` are appended to `result`.
     - `7` is appended to `result`.
   - The final `result` is `[1, 2, 3, 4, 5, 6, 7]`.

4. **Code**
```python
    def flatten_ndarray(arr):
        result = []
        for elem in arr:
            if isinstance(elem, list):
                result.extend(flatten_ndarray(elem))
            else:
                result.append(elem)
        return result
```
   **Example usage**
    arr = [[1, 2, 3], [4, [5, 6]], 7]
    flattened_arr = flatten_ndarray(arr)
    print(flattened_arr)  # Output: [1, 2, 3, 4, 5, 6, 7]

5. **Time Complexity**:
   - The function traverses each element once, so the time complexity is linear: O(n), where n is the total number of elements in the nested array.

6. **Space Complexity**:
   - The recursive call stack and the `result` list contribute to space usage. The maximum depth of recursion depends on the nesting level of the input array.
   - The space complexity is also linear: O(n).

Therefore, the `flatten_ndarray` function efficiently flattens the N-dimensional array into a 1D array.
