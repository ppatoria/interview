# Constructor:
## Question: Constructors should either succeed, wait for a later init() call, or put their object in a well-defined error state ?
### Answer: Yes
This is a good practice because it ensures that objects are always in a valid state after construction.

* **Real world Scenarious.**

    - **Wait for a later init() call**:
    This is often used when the construction of an object requires some information that isn't immediately available.
    In this case, the constructor will create an object in a "not yet initialized" state, and an `init()` method will be provided to fully initialize the object later.
    Here's an example with a `DatabaseConnection` class:

```cpp
class DatabaseConnection {
public:
    DatabaseConnection() : conn(nullptr) {}  // Constructor doesn't initialize the connection

    bool init(std::string db_url) {  // init() method to establish the connection
        conn = establish_connection(db_url);
        return conn != nullptr;
    }

private:
    Connection* conn;
};
```

    In this example, the `DatabaseConnection` object can be created even if the database URL isn't known at construction time.
    The connection is established later with the `init()` method.

2. **Put their object in a well-defined error state**:
   This is used when an error occurs during construction.
   The constructor will catch any exceptions and put the object into a well-defined error state.
   Here's an example with a `FileReader` class:

```cpp
class FileReader {
public:
    FileReader(std::string file_path) {
        file.open(file_path);
        if (!file.is_open()) {
            error_state = true;
        }
    }

    bool in_error_state() const {
        return error_state;
    }

private:
    std::ifstream file;
    bool error_state = false;
};
```

    In this example, if the file cannot be opened (for example, if it doesn't exist), the `FileReader` object is put into an error state, which can be checked with the `in_error_state()` method.

These examples illustrate how constructors in C++ can either succeed, wait for a later `init()` call, or put their object in a well-defined error state.
This ensures that objects are always in a valid state or a well-defined error state after construction, which makes them safer and easier to work with.
It also makes it easier to reason about the behavior of the program, since you can be confident that objects are always in a valid state or a well-defined error state.
This is a fundamental principle of object-oriented programming and is widely used in real-world software development.
