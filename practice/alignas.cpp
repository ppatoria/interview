#include <iostream>

struct alignas(64) OrderNoPadding {
  int price;
  int volume;
};

struct alignas(64) OrderWithPadding {
  int price;
  int volume;
  char padding[64 - sizeof(int) * 2];
};

int main() {
  std::cout << "Size of OrderNoPadding: " << sizeof(OrderNoPadding)
            << " bytes\n";
  std::cout << "Size of OrderWithPadding: " << sizeof(OrderWithPadding)
            << " bytes\n";
}
