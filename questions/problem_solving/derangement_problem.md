# Question: 
Ashish was copying from Rahit in the exam. So, Rahit told him to change the answers a little bit so that the examiner cannot find the fraud. But silly Ashish in the way started to change all the answers that were needed. He shuffled the letters in each word in a way where the maximum number of letters were misplaced.

For a given word, find the maximum difference that Ashish can generate between his answer and Rahit’s answer.

Suppose Rahit wrote “car” for an answer, Ashish can write “acr” with difference 2, or “arc” with differnece 3.

# Solution:
To determine the maximum difference Ashish can generate between his answer and Rahit’s answer by shuffling the letters in each word, we need to calculate the maximum number of misplaced letters possible for a given word. Here's how we can approach this problem:

---

### Key Insight:
The maximum difference occurs when the shuffled word has as many letters as possible in positions different from the original word. This is equivalent to finding the maximum number of letters that can be deranged (i.e., no letter remains in its original position).

---

### Steps to Solve:
1. **Count the frequency of each letter in the word.**
   - This helps in determining how many letters are repeated and how they can be rearranged.

2. **Calculate the maximum number of derangements.**
   - A derangement is a permutation of the letters where no letter appears in its original position.
   - The maximum number of derangements depends on the frequency of each letter. If a letter appears more than once, it becomes easier to derange the word.

3. **Handle repeated letters.**
   - If a letter appears multiple times, it increases the flexibility of derangements. For example, in the word "hello," the letter 'l' appears twice, so we can swap their positions to ensure both are misplaced.

4. **Compute the maximum difference.**
   - The maximum difference is the total number of letters in the word minus the number of letters that cannot be deranged due to their frequency.

---

### Formula:
For a word of length \( n \), the maximum number of misplaced letters is:
\[
\text{Maximum Difference} = n - \text{(number of letters that must remain in their original position)}
\]
If all letters are unique, the maximum difference is \( n \). If there are repeated letters, the maximum difference is reduced because some letters must stay in their original positions to avoid conflicts.

---

### Example:
#### Input: "hello"
- Length of word: 5
- Letter frequencies: h(1), e(1), l(2), o(1)
- The letter 'l' appears twice, so one 'l' must remain in its original position to avoid conflicts.
- Maximum misplaced letters: \( 5 - 1 = 4 \)

#### Output: 4

---

### Algorithm:
1. Count the frequency of each letter in the word.
2. Determine the number of letters that must remain in their original position (this is the number of letters with frequency greater than 1).
3. Subtract this number from the total length of the word to get the maximum difference.

---

### Python Implementation:
```python
from collections import Counter

def max_difference(word):
    freq = Counter(word)
    # Count the number of letters that must remain in their original position
    fixed = sum(1 for count in freq.values() if count > 1)
    # Maximum difference is total length minus fixed letters
    return len(word) - fixed

# Example usage
word = "hello"
print(max_difference(word))  # Output: 4
```

---

### Complexity:
- Time: \( O(n) \), where \( n \) is the length of the word (to count frequencies).
- Space: \( O(1) \), since the frequency dictionary has a fixed size (26 letters for English alphabet).

---

This approach ensures that we calculate the maximum possible difference efficiently.

Here’s the C++ implementation of the solution to find the maximum difference Ashish can generate by shuffling the letters in a word:

```cpp
#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

int maxDifference(const string& word) {
    // Count the frequency of each character in the word
    unordered_map<char, int> freq;
    for (char ch : word) {
        freq[ch]++;
    }

    // Calculate the number of characters that must remain in their original position
    int fixed = 0;
    for (const auto& pair : freq) {
        if (pair.second > 1) {
            fixed++;
        }
    }

    // Maximum difference is the total length minus the number of fixed characters
    return word.length() - fixed;
}

int main() {
    string word;
    cout << "Enter a word: ";
    cin >> word;

    int result = maxDifference(word);
    cout << "Maximum difference: " << result << endl;

    return 0;
}
```

---

### Explanation of the Code:
1. **Input Handling**:
   - The program takes a word as input from the user.

2. **Frequency Counting**:
   - An `unordered_map` is used to count the frequency of each character in the word.

3. **Fixed Characters Calculation**:
   - The number of characters that must remain in their original position is calculated by counting how many characters have a frequency greater than 1.

4. **Maximum Difference Calculation**:
   - The maximum difference is computed as the total length of the word minus the number of fixed characters.

5. **Output**:
   - The result is printed to the console.

---

### Example Run:
#### Input:
```
Enter a word: hello
```

#### Output:
```
Maximum difference: 4
```

---

### Complexity:
- **Time Complexity**: \( O(n) \), where \( n \) is the length of the word. This is because we iterate through the word once to count frequencies and once to calculate fixed characters.
- **Space Complexity**: \( O(1) \), since the frequency map will have at most 26 entries (for the English alphabet).

---

This C++ code efficiently solves the problem and works for any input word.
