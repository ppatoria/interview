#include <iostream>
#include <vector>
template <typename T> class CustomAllocator {
 public:

  using value_type = T;
  
  T* allocate(size_t n){
    if(n == 0) return nullptr;
    void* allocation = ::operator new(sizeof(T)* n);
    if( allocation == nullptr) throw std::bad_alloc();
    return static_cast<T*>(allocation);
  }

  void deallocate(T* ptr, size_t n){
    if(ptr != nullptr) ::operator delete(ptr);
  }

  template<typename U, typename ... Args> 
  void construct(U* ptr, Args&&... args){
    new(ptr) U(std::forward<Args>(args)...);
  }

  template<typename U>
  void destroy(U* ptr) noexcept{
    ptr->~U();
  }
};

int main() {
  std::vector<int, CustomAllocator<int>> vec 
      {
        11,21,51,101,501
      };
  for(const auto& elem : vec){
    std::cout << elem << std::endl;
  }
  return 0;
}
