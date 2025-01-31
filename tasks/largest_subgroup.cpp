
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Question :                                                                                                                                   //
//     Given a sequence of integers and a target sum, find the longest subsequence where the sum of the subsequence is equal to the target sum. //
//     Let's say the sequence is [1, 2, -1, 4, 3, 2, -2, 4] and the target sum is 6.                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <numeric>
#include <span>
#include <array>

const std::span<int>& LongestSubsequence(const std::array<int, 8>& sequence, int expectedSum){
  for (int i = 0; i < sequence.size(); i++) {
    for (int j=0; j< sequence.size(); j++){
      const auto& window = std::span<int>(sequence.data() + i, j - i + 1);
      int sum = std::accumulate(window.begin(), window.end(), 0);
      if(sum == expectedSum){
        return window;
      }
      if(sum > expectedSum){
        break;
      }
    }
  }
  return std::span<int>();
}

int main(){
  std::array<int, 8> arr = {1, 2, -1, 4, 3, 2, -2, 4 };
  int sum = 6;
  const auto &sequence = LongestSubsequence(arr, sum);

  for (const auto& element : sequence){
    std::cout << element << std::endl;
  }
}

  
