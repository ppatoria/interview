# Virtual Functions and VTable

## Question: Is vtable only created when objects are created on heap?
   No, this is not correct.
   The vtable, or virtual table, is a mechanism used by C++ to support dynamic dispatch (or runtime polymorphism).
   When a class has a virtual function, the compiler creates a vtable for that class.
   The vtable is created per class, not per object.
   Therefore, it doesn't matter whether objects are created on the stack or on the heap, the vtable exists as long as the class has at least one virtual function.

    **Flow of Virtual Function Call**:
    - **When derived class object is created on heap and assigned to base class pointer**:
      When a derived class object is created on the heap and a pointer to the base class points to it, the correct overridden function is called based on the actual object type, not the type of the pointer.
      This is because the vtable pointer is set to point to the vtable of the derived class when the derived class object is created.
      When a virtual function is called, the vtable is used to look up the correct function to call.
    - **During object slicing**:
      When a derived class object is assigned to a base class object (slicing), the vtable pointer in the base class object is set to point to the vtable of the base class, not the derived class.
      Therefore, when a virtual function is called on the base class object, it uses the base class's vtable to look up the function, and the base class version of the function is called, not the derived class version.
