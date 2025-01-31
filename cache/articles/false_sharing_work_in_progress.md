
#### **Solutions (Ordered by Efficiency):**
1. **Use `alignas(64)` Padding:** Ensures variables are placed in separate cache lines.
   ```cpp
   struct alignas(64) SharedData {
       int a;
       int b;
   };
   ```
2. **Separate Variables into Different Structures:** Instead of storing both variables in the same structure, place them in separate structures.
   ```cpp
   struct DataA { int a; };
   struct DataB { int b; };
   ```
3. **Use Thread-Local Storage (`thread_local`)**: Ensures each thread has its own copy of the variable.
   ```cpp
   thread_local int a;
   thread_local int b;
   ```
4. **Explicit Memory Allocation with Large Gaps:** Using `std::aligned_alloc` to ensure variables are allocated separately.
   ```cpp
   int* a = static_cast<int*>(std::aligned_alloc(64, sizeof(int)));
   int* b = static_cast<int*>(std::aligned_alloc(64, sizeof(int)));
   ```
5. **Rearrange Workload to Avoid Shared Data:** If possible, modify the algorithm so that each thread works on entirely different data regions.


#### **Diagram**
```plantuml
@startuml
skinparam linetype ortho
rectangle "Core 1" as Core1 {
    rectangle "Private Cache (L1)" as Cache1 {
        rectangle "Cache Line" as CacheLine1 {
            rectangle "Variable A" as VarA
            rectangle "Variable B" as VarB
        }
    }
}
rectangle "Core 2" as Core2 {
    rectangle "Private Cache (L1)" as Cache2 {
        rectangle "Cache Line" as CacheLine2 {
            rectangle "Variable A" as VarA2
            rectangle "Variable B" as VarB2
        }
    }
}
@enduml
```

#### **Code Example**: False Sharing with Separate Variables
```cpp
#include <iostream>
#include <thread>
const int NUM_THREADS = 2;
const int NUM_ITER = 10000000;

struct SharedData {
    int a;  // Thread 1 modifies this
    int b;  // Thread 2 modifies this
} data;

void threadFunc1() {
    for (int i = 0; i < NUM_ITER; ++i) {
        data.a++;
    }
}

void threadFunc2() {
    for (int i = 0; i < NUM_ITER; ++i) {
        data.b++;
    }
}

int main() {
    std::thread t1(threadFunc1);
    std::thread t2(threadFunc2);
    t1.join();
    t2.join();
    std::cout << "Final values: " << data.a << ", " << data.b << std::endl;
}
```

### **Scenario 2: Loop Processing with Separate Indices**
#### **Diagram**
```plantuml
@startuml
skinparam linetype ortho
rectangle "Thread 1" as Thread1
rectangle "Thread 2" as Thread2
rectangle "Cache Line" as CacheLine {
    rectangle "Array[0]" as Array0
    rectangle "Array[1]" as Array1
}
Thread1 --> Array0
Thread2 --> Array1
@enduml
```

### **Scenario 3: Thread-Local Variables Packed Together**
#### **Diagram**
```plantuml
@startuml
skinparam linetype ortho
rectangle "Thread 1 (Core 1)" as T1
rectangle "Thread 2 (Core 2)" as T2
rectangle "Cache Line" as CacheLine {
    rectangle "VarA (Thread 1)" as VarA1
    rectangle "VarA (Thread 2)" as VarA2
}
T1 --> VarA1
T2 --> VarA2
@enduml
```

### **Scenario 4: Struct with Multiple Members Accessed by Different Threads**
#### **Diagram**
```plantuml
@startuml
skinparam linetype ortho
rectangle "Cache Line" as CacheLine {
    rectangle "Struct.A" as StructA
    rectangle "Struct.B" as StructB
}
rectangle "Thread 1" as T1
rectangle "Thread 2" as T2
T1 --> StructA
T2 --> StructB
@enduml
```

## **Conclusion**
False sharing significantly degrades multi-threaded performance by causing unnecessary cache invalidations. Proper data alignment, padding, and struct organization can effectively mitigate the issue, leading to more efficient multi-core computing.

