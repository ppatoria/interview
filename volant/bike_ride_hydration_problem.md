# Bike ride hydration problem

A cyclist rides a bike on a long, straight bicycle path.
He has one water bottle- initially full.
With a full bottle, he can ride at most "range" kilometres before passing out from dehydration.
Please implement the C++20 function...
```c++
int min_refills(int start_pos, int finish_pos, int range, const std::vector<int>& tap_positions)
...
```
such that it efficiently (in a big-O sense) calculates and returns the minimum number of times he'd need to refill his water bottle when riding from the "start" kilometre mark to the "finish" kilometre mark.
The kilometre positions of each tap where he can refill his bottle are provided in the "taps" argument.
If it's not possible to reach the intended finish mark, min_refills should return -1.
* **For example**, a call to...
```c++
min_refills(10, 20, 4, {13, 17, 18, 22, 7 30})
```
models a desire to ride from the 10km mark and reach the 20km mark, an ability to ride 4 kms between refills, and taps at each of the 13, 17, 18, 22, and 30 kilometre marks.
Starting at the 10km mark, after refilling twice - at the 13km and 17km marks - he'll have enough water to reach his finish point at the 20km mark, so min_refills should return 2.

* **Another examples:**
min_refills(10, 25, 5, {13, 17, 18, 21, 22, 30}) should refill at 13, 17 or 18, 21 or 22kms => result 3.
NOTES
- all kilometre marks - start_pos, finish_pos and tap_positions (if any) - are in the range [0..1,000,000] inclusive - start <= finish, and tap_positions are distinct values in increasing order - range is in the range [1..1,000,000] km .
Please implement  the below min_refills function  in c++20 and it should be as efficient at it could be.
```c++
#include <bits/stdc++.h>

using namespace std;
int min_refills(int start_pos, int finish_pos, int range, const vector<int>& tap_positions) {
    // please implement this function...

}
int main()
{
    int start_pos, finish_pos, range;
    std::vector<int> tap_positions;

    assert(cin >> start_pos >> finish_pos >> range);
    int temp;
    while (cin >> temp) tap_positions.push_back(temp);
    assert(cin.eof());

    int result = min_refills(start_pos, finish_pos, range, tap_positions);

    if (std::ofstream fout{getenv("OUTPUT_PATH")})
        fout << result << "\n";
    else
        return EXIT_FAILURE;
}
```
## Solution:
### Problem Explanation:
The problem is about a cyclist who is riding on a long, straight bicycle path.
The cyclist has a water bottle that, when full, allows him to ride a certain distance before he needs to refill it.
The distance he can ride with a full bottle is given by the variable `range`.

The cyclist starts at a certain position on the path (`start_pos`) and wants to reach another position (`finish_pos`).
Along the path, there are several taps where he can refill his water bottle.
The positions of these taps are given in the `tap_positions` vector.

The goal is to find the minimum number of times the cyclist needs to refill his water bottle in order to reach his destination.
If it's not possible for him to reach his destination (because the taps are too far apart), the function should return -1.

Here's a simple example to illustrate the problem:

Let's say the cyclist starts at position 10 (`start_pos = 10`), wants to reach position 20 (`finish_pos = 20`), and can ride 4 km with a full bottle (`range = 4`).
There are taps at positions 13, 17, 18, 22, and 30 (`tap_positions = {13, 17, 18, 22, 30}`).

The cyclist starts at position 10 with a full bottle.
He can ride 4 km before needing to refill, so he can reach position 14.
The nearest tap is at position 13, so he stops there and refills his bottle.

Now, with a full bottle, he can ride another 4 km, which takes him to position 17.
There's a tap at this position, so he refills his bottle again.

With another full bottle, he can ride 4 km to position 21.
However, his destination is at position 20, so he can stop there without needing to refill his bottle again.

So, in total, the cyclist needed to refill his bottle 2 times to reach his destination.
Therefore, the function `min_refills(10, 20, 4, {13, 17, 18, 22, 30})` should return 2.
### Algorithm:
1. **Initialization**:
- Start by initializing a variable, say `current_pos`, to `start_pos`. This variable will keep track of the current position of the cyclist.
- Also, initialize a variable, say `refills`, to 0. This variable will keep track of the number of refills.

2. **Traversal**: Traverse through the `tap_positions` vector.
For each tap position, check if the cyclist can reach it from `current_pos` with the water he currently has (i.e., if the difference between the tap position and `current_pos` is less than or equal to `range`). 
If he can, continue to the next tap position.
If he can't, it means he needs to refill at the previous tap position.
In this case, increment `refills` by 1 and update `current_pos` to the previous tap position.

3. **Final Check**:
After the traversal, check if the cyclist can reach `finish_pos` from `current_pos` with the water he currently has.
- If he can, return `refills` as the minimum number of refills needed.
- If he can't, it means he needs to refill at the last tap position. In this case, increment `refills` by 1 and return it.

4. **No Solution**: If at any point during the traversal or the final check, the cyclist needs to refill but there's no tap position to do so, return -1 to indicate that it's not possible to reach the finish position.

This algorithm traverses the `tap_positions` vector only once, so its time complexity is O(n), where n is the number of tap positions.
It also uses a constant amount of space, so its space complexity is O(1).
#### Test Algorithm:
Let's walk through the algorithm with the first example you provided:

```c++
min_refills(10, 20, 4, {13, 17, 18, 22, 7, 30})
```

Here, the cyclist starts at the 10km mark and wants to reach the 20km mark.
He can ride 4 kms between refills, and there are taps at the 13, 17, 18, 22, and 30 kilometre marks.

**Step 1 - Initialization**:
- `current_pos` = 10 (start_pos)
- `refills` = 0

**Step 2 - Traversal**:
- The first tap position is 13. The cyclist can reach it from `current_pos` (10) because 13 - 10 = 3, which is less than `range` (4).
  - continue to the next tap position.
- The next tap position is 17. The cyclist can't reach it from `current_pos` (10) because 17 - 10 = 7, which is greater than `range` (4). So, he needs to refill at the previous tap position (13).
  - increment `refills` by 1 and
  - update `current_pos` to 13.
- The next tap position is 18. The cyclist cannot reach it from current_pos (13) because 18 - 13 = 5, which is greater than range (4). So, he needs to refill at the previous tap position (17).
  - increment refills by 1 and
  - update current_pos to 17.
- The next tap positions are 22 and 30, but they're beyond the finish position, so we ignore them.

**Step 3 - Final Check**:
- After the traversal, we check if the cyclist can reach `finish_pos` (20) from `current_pos` (17).
  He can because 20 - 17 = 3, which is less than `range` (4).
  - return `refills` (2) as the minimum number of refills needed.

So, `min_refills(10, 20, 4, {13, 17, 18, 22, 7, 30})` returns 2, which means the cyclist needs to refill once to reach the 20km mark from the 10km mark.

### Implementation:
The idea is to keep track of the last refill position and continuously look ahead in the tap_positions to the farthest tap that can be reached.
If a tap cannot be reached, return -1.

```c++
#include <bits/stdc++.h>

using namespace std;

int min_refills(int start_pos, int finish_pos, int range, const vector<int>& tap_positions) {
    int n              = tap_positions.size();
    int num_refills    = 0;
    int current_refill = 0;
    int current_pos    = start_pos;

    while (current_pos + range < finish_pos) {
    /** While condition (current_pos + range < finish_pos) checks if the cyclist can reach the finish line with the water he currently has.
        If he can’t (current_pos + range is less than finish_pos)
            the loop continues, and the cyclist needs to find a tap to refill his water bottle.
        If he can (current_pos + range is greater than or equal to finish_pos)
            the loop ends, and the function returns the number of refills the cyclist needed to reach the finish line.
    */

        if (current_refill >= n || tap_positions[current_refill] > current_pos + range)
        { return -1; }
        /** The condition `if (current_refill >= n || tap_positions[current_refill] > current_pos + range)` is a safety check that happens at the start of each loop iteration. It checks two things:
            1. `current_refill >= n`: This checks if the cyclist has already considered all the tap positions.
               If `current_refill` is equal to or greater than `n` (the total number of tap positions), it means there are no more taps to consider.
                   In this case, the cyclist cannot reach the next refill station and the function returns -1.
            2. `tap_positions[current_refill] > current_pos + range`: This checks if the next tap position is within reach from the cyclist's current position.
               If the next tap (`tap_positions[current_refill]`) is further than what the cyclist can reach with his current water supply (`current_pos + range`), it means the cyclist cannot reach the next refill station and the function returns -1.

            This condition is checking if it's possible for the cyclist to continue.
            If it's not (because there are no more taps or the next tap is out of reach)
                the function returns -1 to indicate that it's not possible to reach the finish position.
        */

        while (current_refill < n-1 && tap_positions[current_refill+1] <= current_pos + range)
        { current_refill++; }
        /** The condition `while (current_refill < n-1 && tap_positions[current_refill+1] <= current_pos + range)` is inside the main loop of the function.
            It's used to find the furthest tap position that the cyclist can reach from his current position with the water he currently has.
            Here's what each part means:
              - `current_refill < n-1`:
                  This checks if there are more tap positions to consider.
                  If `current_refill` is equal to or greater than `n-1` (the total number of tap positions minus 1), it means there are no more taps to consider.
              - `tap_positions[current_refill+1] <= current_pos + range`:
                  This checks if the next tap position is within reach from the cyclist's current position.
                      If the next tap (`tap_positions[current_refill+1]`) is less than or equal to the furthest position the cyclist can reach with his current water supply (`current_pos + range`), it means the cyclist can reach the next tap.

            This inner while loop continues as long as there are more taps to consider and the next tap is within reach.
            During each iteration of the loop, `current_refill` is incremented, effectively moving to the next tap position.
            This way, the loop always finds the furthest tap position that the cyclist can reach, which helps minimize the number of refills.
        */

        current_pos = tap_positions[current_refill];
        num_refills++;
        current_refill++;
    }

    return num_refills;
}

int main() {
    int start_pos, finish_pos, range;
    std::vector<int> tap_positions;

    assert(cin >> start_pos >> finish_pos >> range);
    int temp;
    while (cin >> temp) tap_positions.push_back(temp);
    assert(cin.eof());

    int result = min_refills(start_pos, finish_pos, range, tap_positions);

    if (std::ofstream fout{getenv("OUTPUT_PATH")})
        fout << result << "\n";
    else
        return EXIT_FAILURE;
}
```
This function iterates through the tap_positions vector only once, so it runs in O(n) time, where n is the number of tap positions.
This is as efficient as it cold be for this problem.
#### Test Implementation:
Let's walk through the function with the example `min_refills(10, 20, 4, {13, 17, 18, 22, 7, 30})`.

Here, the cyclist starts at the 10km mark and wants to reach the 20km mark.
He can ride 4 kms between refills, and there are taps at the 13, 17, 18, 22, 7, and 30 kilometre marks.

**Step 1 - Initialization**:
- `current_pos` = 10 (start_pos)
- `num_refills` = 0
- `current_refill` = 0

**Step 2 - Main Loop**:
- The condition for the while loop is `current_pos + range < finish_pos`, which is `10 + 4 < 20`, so we enter the loop.
- The first if condition checks if `current_refill >= n` or `tap_positions[current_refill] > current_pos + range`.
  Since `current_refill` is 0 and `tap_positions[0]` is 13, which is not greater than `current_pos + range` (14), we don't enter the if block.
- We then enter the inner while loop, which continues as long as `current_refill < n-1` and `tap_positions[current_refill+1] <= current_pos + range`.
  Since `tap_positions[1]` (17) is not less than or equal to `current_pos + range` (14), we don't enter the inner while loop.
- We then update `current_pos` to `tap_positions[current_refill]` (13) and increment `num_refills` and `current_refill` by 1.

**Step 3 - Next Iteration**:
- We again check the condition for the while loop, which is now `13 + 4 < 20`, so we enter the loop.
- The first if condition is again false because `tap_positions[1]` (17) is not greater than `current_pos + range` (17).
- We then enter the inner while loop. Since `tap_positions[2]` (18) is greater than `current_pos + range` (17), we don't enter the inner while loop.
- We then update `current_pos` to `tap_positions[current_refill]` (17) and increment `num_refills` and `current_refill` by 1.

**Step 4 - Final Check**:
- We again check the condition for the while loop, which is now `17 + 4 < 20`, so we don't enter the loop.
- We then return `num_refills` (2) as the minimum number of refills needed.

So, `min_refills(10, 20, 4, {13, 17, 18, 22, 7, 30})` returns 2, which means the cyclist needs to refill twice to reach the 20km mark from the 10km mark.

I hope this helps you understand the function better! If you have any more questions, feel free to ask. 😊





