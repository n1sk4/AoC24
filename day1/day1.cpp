#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <numeric>

int main()
{
  std::ifstream file("D:\\AoC24\\day1\\input.txt");
  if (!file)
  {
    std::cerr << "Unable to open file\n";
    return 1;
  }

  std::vector<int> left;
  std::vector<int> right;

  int col1, col2;
  while (file >> col1 >> col2)
  {
    left.push_back(col1);
    right.push_back(col2);
  }

  file.close();

  std::sort(left.begin(), left.end());
  std::sort(right.begin(), right.end());

  std::vector<int> result1(left.size());

  std::transform(left.begin(), left.end(), right.begin(), result1.begin(),
                 [](int a, int b)
                 { return abs(a - b); });

  int solution1 = std::accumulate(result1.begin(), result1.end(), 0);
  
  std::vector<int> result2(left.size());

  for(auto l : left)
  {
    int count = std::count(right.begin(), right.end(), l);
    result2.push_back(count * l);
  }

  int solution2 = std::accumulate(result2.begin(), result2.end(), 0);
  
  // PART 1 solution
  std::cout << "Part 1 solution: total distance: '" << solution1 << "'\n";

  // PART 2 solution
  std::cout << "Part 2 solution: similarity score: '" << solution2 << "'\n";

  system("pause");
  return 0;
}
