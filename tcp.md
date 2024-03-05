# N-byte message:

  This means that the server is designed to receive a message that is exactly N bytes long.
  It could be a single message or a protocol that dictates that every message should be N bytes.
  If messages are not exactly this length, the server might need to handle incomplete or multiple messages accordingly.

# Memory-page sized buffer:
  Operating systems manage memory in units called "pages."
  A memory-page sized buffer refers to a buffer that is the same size as a memory page, which is typically a power of 2, like 4KB (4096 bytes).
  This size can be efficient for system memory management and can align with the underlying hardware architecture.

# File Descriptor (FD) and Socket Descriptor (Socket FD)
  In the context of TCP/IP networking, a file descriptor (FD) and a socket file descriptor (Socket FD) are related but serve different purposes:

  - **File Descriptor (FD)**               : This is a general term used to refer to an abstract indicator for accessing a file or other input/output resource, such as a pipe or network socket.
                                             It's an integer that the operating system uses to uniquely identify an open file within a process.
  - **Socket File Descriptor (Socket FD)** : This is a specific type of file descriptor that refers to a network socket.
                                             A network socket is an endpoint for sending or receiving data across a computer network, particularly in the context of TCP/IP networking.

  **Key Differences**:
  - **Usage**      : A regular FD can be used for files, devices, pipes, etc., while a Socket FD is specifically used for network communication.
  - **Binding**    : A Socket FD is typically bound to a network address and port, which is necessary for network communication. A regular FD doesn't have this concept unless it's a Socket FD.
  - **Operations** : The operations you can perform on a Socket FD are specific to network communication, such as `connect`, `listen`, `accept`, `send`, and `recv`.
                     A regular FD has operations like `read` and `write`, which can also be used on a Socket FD but have different semantics.

  In summary, while both are file descriptors, a Socket FD is a specialized type used for network operations within the TCP/IP stack, and it includes additional functionalities related to networking.
# Blocking read:
  As explained, the read operation will wait for data to be available.

  In blocking mode, when a read operation is performed on a socket, the program will wait (or "block") until the specified number of bytes has been read.
  This means the program's execution will not continue until the read operation completes.

  Here's an example of a blocking read in C++20:

  ```cpp
  #include <sys/socket.h>
  #include <unistd.h>

  // ... (socket creation and connection code)

  char buffer[4096];
  ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
  // The program will block here if there are no 4096 bytes available to read.
  ```
# Non-blocking read:
  In non-blocking mode, the read operation will return immediately, even if no data is available.
  If no data is available, the read function will return -1, and `errno` will be set to `EAGAIN` or `EWOULDBLOCK`.

  Here's how you might set a socket to non-blocking mode and perform a non-blocking read:

  In a non-blocking read operation, the read function will return immediately if there is no data available, or it will return as much data as is available at the moment, up to the requested size.
  For a 3000-byte message, the read function may return less than 3000 bytes if the full message has not yet been received.
  It’s the application’s responsibility to handle partial messages and to continue reading until the full message has been received.

  Here’s how you might handle reading a 3000-byte message in non-blocking mode:

  ```cpp
  #include <fcntl.h>
  #include <errno.h>

  // ... (socket creation and connection code)

  // Set the socket to non-blocking mode
  int flags = fcntl(client_fd, F_GETFL, 0);
  fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

  //char buffer[4096];
  char message[3000];
  ssize_t total_bytes_read = 0;
  ssize_t bytes_read;

  while (total_bytes_read < 3000) {
      bytes_read = read(client_fd, message + total_bytes_read, 3000 - total_bytes_read);

      if (bytes_read > 0) {
          total_bytes_read += bytes_read;
      } else if (bytes_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
          // No data available right now, try again later
          // You can use select(), poll(), or epoll() to wait for data
      } else {
          // An error occurred
          break;
      }
  }

  if (total_bytes_read == 3000) {
      // Full message has been read
  }
    ```
  In this example, the loop continues to attempt to read from the socket until the full 3000 bytes have been read.
  If read returns -1 and errno is set to EAGAIN or EWOULDBLOCK, it means no data is available at the moment, and you should try reading again later.
  This might involve using a multiplexing system call like select(), poll(), or epoll() to wait for the socket to become readable.

  Remember, in non-blocking mode, you must be prepared to handle partial reads and call read multiple times to receive the full message.
  The C++ code snippet is for setting up a non-blocking socket and attempting to read data from it. Here's what each part does:

  1. **Include Headers**: `fcntl.h` and `errno.h` are included for the `fcntl` function and `errno` variable, respectively.
    ```cpp
    #include <fcntl.h>
    #include <errno.h>
    ```

  2. **Set Socket to Non-Blocking Mode**:
    - `fcntl(client_fd, F_GETFL, 0)` retrieves the current flags set on the socket referred to by `client_fd`.
    - `fcntl(client_fd, F_SETFL, flags | O_NONBLOCK)` sets the socket to non-blocking mode by OR-ing the current flags with `O_NONBLOCK`.
    ```cpp
    int flags = fcntl(client_fd, F_GETFL, 0);
    fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
    ```

  4. **Buffer Declaration**: A buffer of 4096 bytes (typically the size of a memory page) is declared to store the data read from the socket.
    ```cpp
    char buffer[4096];
    ```

  5. **Read Data**:
    - `read(client_fd, buffer, sizeof(buffer))` attempts to read up to 4096 bytes from the socket into the buffer.
    - `ssize_t bytes_read` stores the number of bytes actually read, or `-1` if an error occurred.
    ```cpp
    ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
    ```

  6. **Error Handling**:
    - If `bytes_read` is `-1`, an error occurred during the read operation.
    - If `errno` is set to `EAGAIN` or `EWOULDBLOCK`, it means no data was available to read, and the operation would block if the socket were not in non-blocking mode.
    - Otherwise, a different error occurred, and it should be handled accordingly.
    ```cpp
    if (bytes_read == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            // No data available right now, try again later
        } else {
            // Handle other errors
        }
    }
    ```

  In summary, this code sets up a non-blocking socket, tries to read data from it, and handles the case where no data is available without blocking the program's execution.
  If no data is available, the program can continue doing other tasks and check back later.
  In this non-blocking example, if `read` returns -1 and `errno` is set to `EAGAIN` or `EWOULDBLOCK`.
  It indicates that there is no data available at the moment, and the program can decide to do something else while waiting for data to arrive.

  In the case of a non-blocking read call, the behavior is quite different from a blocking read. Here's what happens with a non-blocking read:

  - **Immediate Return**   : The `read` function will return immediately if there is no data available to be read from the socket. It doesn't wait for data to arrive.
  - **No Separate Thread** : Non-blocking I/O doesn't involve creating a separate thread to handle the read operation. The read operation occurs in the same thread that made the call.
  - **No Signal Event**    : There's no signaling mechanism inherent to the `read` function that notifies you when the operation is complete.
                             Instead, you would typically use other mechanisms like `select`, `poll`, or `epoll` to monitor the socket for readability.

  Here's an example of how you might use `poll` to wait for data to become available on a non-blocking socket:

  ```cpp
  #include <poll.h>

  // ... (socket creation and connection code)

  struct pollfd fds;
  int ret;

  fds.fd = client_fd; // your socket descriptor
  fds.events = POLLIN; // check for normal data (not out-of-band data)

  // Use poll to wait for the event or a timeout
  ret = poll(&fds, 1, TIMEOUT_MILLISECONDS);

  if (ret > 0) {
      // Data is available to read
      ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
      // Process the read data
  } else if (ret == 0) {
      // The timeout was reached, no data is available
  } else {
      // An error occurred
  }
  ```

  In this code:
  - `poll` is used to wait for the socket to become readable (`POLLIN` event).
  - If `poll` returns a positive number, it means the socket has data available to read.
  - If it returns `0`, the specified timeout was reached without any data becoming available.
  - If it returns `-1`, an error occurred during the poll.

  This approach allows you to efficiently manage non-blocking sockets, especially when dealing with multiple connections simultaneously.

## Non-blocking using seperate thread:

  Polling in a separate thread is indeed one way to achieve non-blocking behavior while still being able to process incoming data as it arrives.
  Here's how it works:

  - **Separate Thread** : You create a separate thread dedicated to handling I/O operations.
                          This thread can use polling mechanisms like `select`, `poll`, or `epoll` to monitor multiple sockets for readability or writability without blocking the main thread of your application.
  - **Main Thread**     : The main thread of your application continues to execute other tasks, such as UI updates or computational work, without being blocked by I/O operations.
  - **Communication**   : The I/O thread communicates with the main thread using thread-safe mechanisms such as condition variables, mutexes, or message queues to signal when data is available or when an I/O operation has completed.

  Here's a simplified example using C++20 features to demonstrate this concept:

  ```cpp
  #include <thread>
  #include <vector>
  #include <iostream>
  #include <poll.h>
  #include <unistd.h>

  // Function that the I/O thread will run
  void io_thread_func(int client_fd) {
      struct pollfd fds;
      fds.fd = client_fd;
      fds.events = POLLIN;

      while (true) {
          int ret = poll(&fds, 1, -1); // Timeout set to -1 for infinite wait
          if (ret > 0) {
              // Data is available to read
              char buffer[3000];
              ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer));
              // Process the read data
              // Communicate with the main thread if necessary
          } else if (ret == 0) {
              // Timeout reached, handle if needed
          } else {
              // An error occurred
              break;
          }
      }
  }

  int main() {
      // Assume client_fd is the file descriptor for the non-blocking socket
      int client_fd = /* ... */;

      // Start the I/O thread
      std::thread io_thread(io_thread_func, client_fd);

      // Main thread continues to do other work
      // ...

      // Wait for the I/O thread to finish before exiting the program
      io_thread.join();

      return 0;
  }
  ```

  In this example, the `io_thread_func` function contains the polling logic and is executed in a separate thread created in the `main` function.
  The main thread can continue running other code without being blocked by the I/O operations happening in `io_thread_func`.

  Using a separate thread for polling allows your application to handle I/O in a non-blocking manner while the main thread remains responsive.
  However, it's important to carefully design the communication between threads to avoid issues like race conditions and deadlocks.

## The techniques for handling non-blocking I/O operations without separate thread:

### I/O Multiplexing**:

  Using I/O multiplexing with `select`, `poll`, or `epoll` doesn't inherently require a separate thread to raise events.
  These functions are designed to monitor multiple file descriptors within a single thread and can block until one or more of the file descriptors become ready for some class of I/O operation.

  Here's how it works in a single-threaded environment:
  - **What** : It's like having a receptionist who watches multiple phone lines and lets you know which one is ringing.
  - **How**  : You use `select`, `poll`, or `epoll` to monitor multiple sockets.
  - **Flow** :
    1. **Prepare** : You prepare a list of file descriptors (sockets) that you want to monitor.
    2. **Call**    : You call `select`, `poll`, or `epoll`, which will block execution in the current thread.
    3. **Wait**    : The function waits until there's an event (like data being available for reading) on any of the monitored file descriptors.
    4. **Return**  : Once an event occurs, the function returns control to your program with information about which file descriptors have events.
    5. **Handle**  : Your program then handles the events accordingly.

  In a single-threaded model, while the function is waiting, your program isn't doing anything else—it's blocked. This is fine for many applications, especially those that are I/O-bound and don't have much other work to do while waiting for I/O.

  However, if you need to perform other tasks while waiting for I/O events, you might use non-blocking mode with these functions, or indeed, use a separate thread or process as you mentioned, or even look into asynchronous I/O techniques that don't block the thread at all.👨‍💻

  In the context of I/O multiplexing and non-blocking I/O operations, the operating system (OS) or kernel is responsible for monitoring the file descriptors and raising the events.

  - **Operating System Role**   : The OS manages all I/O operations at a low level.
                                  When you use functions like `select`, `poll`, or `epoll`, you're asking the OS to watch the specified file descriptors for any activity, such as incoming data ready to be read.
  - **Event Notification**      : When the OS detects that there's activity on one of the monitored file descriptors, it notifies your program by making the `select`, `poll`, or `epoll` function return.
                                  This is not done in parallel by your application but by the OS, which is continuously managing multiple processes and their I/O in parallel.
  - **Handling in Your Program**: Once the function returns, your program checks which file descriptor(s) have activity and handles them accordingly.
                                  This happens sequentially in your program's flow, even though the OS is handling many such events in parallel for various processes.

  So, while your application is not running a separate thread to raise these events, the OS is indeed handling these activities "in parallel" in the sense that it's managing multiple processes and their I/O needs concurrently.

### Asynchronous I/O (AIO):
  With Asynchronous I/O (AIO), you’re dealing with individual file descriptors (FDs) for each I/O operation,
  whereas with I/O Multiplexing, you’re monitoring a list of FDs to see which ones are ready for I/O.

  In the context of Asynchronous I/O (AIO), the I/O request is submitted to the operating system's kernel.
  The kernel has a specific AIO interface that handles these requests. Here's a more detailed flow:

   - **What**: It's like dropping off your laundry at a service; they'll do the work, and you pick it up when it's done.
   - **How**: You submit I/O requests that the system processes in the background.
   - **Flow**:
     1. **Submit Request**: You use AIO functions to submit your I/O request to the kernel.
     2. **Kernel Processes Request**: The kernel schedules and processes the I/O operation in the background, separate from your application's process.
     3. **Notification**: Once the I/O operation is complete, the kernel notifies your application. This could be through a callback function, a signal, or by polling for the completion status.


  If you were to use AIO with network sockets, the process would be:
  1. **Create and Bind Socket** : You create a socket and bind it to a port as you would in a synchronous operation.
  2. **Set Up AIO**             : Instead of using the regular `read` or `write` system calls, you set up AIO operations with the bound socket's file descriptor.
  3. **Kernel Receives Data**   : The kernel receives incoming data on the bound port and handles it according to the AIO requests you've submitted.

  So, the binding to a port is still a necessary step for network communication, even when using AIO.
  The difference with AIO is how the data read or write operations are handled after the socket is ready for I/O.

  **Under the Hood** : With AIO, you submit a specific I/O operation request to the kernel, such as reading from or writing to a file descriptor.
  **Kernel’s Role**  : The kernel schedules this I/O operation independently of the process’s main execution flow. It handles the operation in the background, using its own mechanisms and threads.
  **Notification**   : Once the I/O operation is complete, the kernel notifies the process. This can be done via a callback function, which is indeed run asynchronously. The callback is not run in a kernel thread but is scheduled by the kernel to run in the context of the process that initiated the AIO operation. 

### I/O Multiples vs AIO

| Aspect                | I/O Multiplexing                                                                                | Asynchronous I/O (AIO)                                                                                |
|-----------------------|-------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------|
| **Definition**        | Allows a process to monitor multiple file descriptors to see if I/O is possible on any of them. | Enables a process to perform I/O operations without waiting for them to complete,                     |
|                       |                                                                                                 | with the kernel handling these operations in the background.                                          |
| **Mechanism**         | Uses `select`, `poll`, or `epoll` to check multiple sockets for readiness.                      | Uses AIO system calls like `aio_read` or `aio_write`                                                  |
|                       |                                                                                                 | to submit I/O requests to the kernel.                                                                 |
| **Kernel's Role**     | Monitors the specified file descriptors and                                                     | Manages and schedules the I/O operation independently of the process's main execution flow.           |
|                       | returns control to the program when I/O is ready.                                               |                                                                                                       |
| **Notification**      | The process checks the return value                                                             | The process is notified via a callback function or by checking the status of the operation.           |
|                       | of `select`, `poll`, or `epoll` to determine which file descriptors are ready.                  |                                                                                                       |
| **Setup**             | Requires setting up file descriptor sets and possibly setting sockets to non-blocking mode.     | Requires setting up AIO control blocks with details of the I/O operation.                             |
| **Use Case**          | Ideal for scenarios where a process needs to handle I/O on multiple sockets efficiently.        | Suitable for high-performance I/O operations where multiple I/O requests can be handled concurrently. |
| **APIs/Terminology**  | `select`, `poll`, `epoll`                                                                       | `aio_read`, `aio_write`, `aiocb` (AIO control block)                                                  |
| **Binding to Port**   | In networking, sockets must be bound to ports to receive data.                                  | In networking, sockets must be bound to ports to receive data.                                        |
| **Execution Context** | The process may block if it uses blocking mode,                                                 | Callbacks are executed asynchronously when the process is scheduled by the kernel.                    |
|                       | or it may use non-blocking mode with these functions.                                           |                                                                                                       |
|-----------------------|-------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------------|

### Signal-Driven I/O:
   - **What**: It's like setting an alarm that alerts you when your cake is ready in the oven.
   - **How**: You configure a socket to send a signal when I/O is possible.
   - **Flow**: 
     1. You set up the socket with signal notifications.
     2. You work on other tasks.
     3. A signal interrupts your work when data is ready.

  **Under the Hood**   : When you set up a socket for signal-driven I/O, you’re telling the kernel to monitor the socket and send a signal (like SIGIO) to your process when data is available.
  **Kernel’s Role**    : The kernel keeps an eye on the socket as part of its regular task scheduling and I/O management.
                         When it detects that the socket is ready for I/O, it interrupts the process by sending the configured signal.
  **Process Handling** : The process needs to have a signal handler set up to catch this signal. When the signal is received, the signal handler function is executed synchronously within the context of the process.
                         This means the process stops its current execution flow to handle the signal.
### Asynchronous vs Signal-Driven IO:

| Aspect                | Asynchronous I/O (AIO)                                                                                | Signal-Driven I/O                                                                          |
|-----------------------|-------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------|
| **Definition**        | AIO allows processes to initiate I/O operations without waiting for them to complete.                 | Signal-Driven I/O notifies the process via a signal                                        |
|                       | The kernel handles these operations in the background.                                                | when the kernel is ready to perform I/O operations without blocking the process.           |
| **Mechanism**         | Uses AIO control blocks and system calls like `aio_read` or `aio_write` to manage I/O operations.     | Uses signals (like `SIGIO`) to alert the process that I/O is ready on a file descriptor.   |
| **Kernel's Role**     | Manages and schedules the I/O operation independently of the process's main execution flow.           | Monitors the file descriptor and sends a signal to the process when I/O is ready.          |
| **Notification**      | The process is notified via a callback function or by checking the status of the operation.           | The process receives a signal and must have a signal handler to respond to it.             |
| **Setup**             | Requires setting up AIO control blocks with details of the I/O operation.                             | Requires setting the socket to non-blocking mode and enabling signal-driven notifications. |
| **Use Case**          | Suitable for high-performance I/O operations where multiple I/O requests can be handled concurrently. | Useful for simpler scenarios where                                                         |
|                       |                                                                                                       | the process needs to be notified about I/O readiness without continuous polling.           |
| **APIs/Terminology**  | `aio_read`, `aio_write`, `aiocb` (AIO control block)                                                  | `fcntl` with `F_SETFL` and `O_ASYNC`, `SIGIO`                                              |
| **Binding to Port**   | In the context of networking, sockets still need to be bound to ports to receive data.                | Sockets are explicitly created and bound to ports to listen for incoming connections.      |
| **Execution Context** | Callbacks are executed asynchronously when the process is scheduled by the kernel.                    | Signal handlers are executed synchronously within the process's context.                   |
|-----------------------|-------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------------------|


### Non-Blocking I/O with Callbacks:
   - **What**: It's like having a callback service that calls you back when it's your turn.
   - **How**: Libraries provide functions that call your code back when I/O is ready.
   - **Flow**:
     1. You register a callback function for I/O events.
     2. You do other work.
     3. The callback gets called when the event occurs.

  In the context of **Non-Blocking I/O with Callbacks**, here's how the responsibilities are typically divided between the library and the application:

  | Part                  | Handled by Library                                                                             | Handled by Application                                                                              |
  |-----------------------|------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
  | **I/O Operations**    | Provides abstractions for non-blocking I/O operations and manages the underlying system calls. | Uses the library's API to initiate I/O operations without dealing directly with the system calls.   |
  | **Event Loop**        | Implements the event loop that waits for I/O events                                            | Registers interest in specific I/O events and provides the event loop with the necessary callbacks. |
  |                       | and dispatches them to the appropriate handlers.                                               |                                                                                                     |
  | **Callbacks**         | Calls the registered callbacks when I/O events occur.                                          | Implements the callbacks to handle I/O events (e.g., data ready to read, socket ready to write).    |
  | **Error Handling**    | Handles errors that occur within the scope of the library's operations                         | Handles errors reported by the library and implements application-specific error recovery logic.    |
  |                       | and provides mechanisms to report these errors.                                                |                                                                                                     |
  | **Concurrency**       | May provide features to handle concurrency,                                                    | Decides how to handle concurrency in the context of the application,                                |
  |                       | such as thread pools or asynchronous task execution.                                           | which may involve coordinating with the library's concurrency features.                             |
  | **Socket Management** | May offer utilities for creating, binding, and managing sockets.                               | Responsible for setting up sockets using the library's utilities and managing their lifecycle.      |
  | **Data Processing**   | -                                                                                              | Processes the data received through I/O operations and performs application-specific logic.         |
  |-----------------------|------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------|
  The library's role is to simplify the complexity of non-blocking I/O by providing a higher-level interface for the application to use.
  The application's role is to implement the business logic, handle the data processing, and interact with the library to perform I/O operations.

  In the context of networking and Non-Blocking I/O, an event loop provided by a library (such as Boost asio) is a mechanism that allows the application to perform I/O operations without blocking the main thread
  **Here's how library works and the types of events it handles:**

  - **Event Loop**: It's a loop that continuously checks for I/O events and dispatches them to be handled.
                    It ensures that I/O operations do not block the main thread by offloading the operations and monitoring their completion.

  - **Events Handled by the Event Loop**:
    - **I/O Readiness**          : When a socket is ready for reading or writing, the event loop is notified. The application can then process the incoming data or send data without blocking.
    - **New Connections**        : In the case of a server, the event loop will notify the application of new incoming connections, allowing the application to accept and handle them.
    - **Timeouts and Intervals** : The event loop can handle timeouts and intervals, triggering callbacks after a specified time has elapsed.
    - **Asynchronous Tasks**     : Tasks that are scheduled to run asynchronously, such as DNS lookups or file operations, are also managed by the event loop.

  - **Event Loop Processing**:
    1. **Polling**             : The event loop polls for I/O events using mechanisms like `epoll` or `kqueue`.
    2. **Queueing**            : Once an event is detected, it's queued up to be processed.
    3. **Callback Invocation** : The event loop invokes the callback associated with the event. This is done without blocking the main thread.
    4. **Continuation**        : After the callback is executed, the event loop continues to poll for more events.

  The library abstracts the complexity of these operations, providing a simple interface for the application to use.
  The application registers callbacks and handles events as they occur, focusing on the logic rather than the intricacies of non-blocking I/O.

  **Under the hood, libraries providing Non-Blocking I/O, such as Boost.Asio** typically use a combination of techniques to achieve non-blocking behavior. These can include:
    - **Event Demultiplexing**    : Using system calls like `select`, `poll`, or `epoll` to monitor multiple streams for events.
    - **Asynchronous Operations** : Some may use the OS's native AIO capabilities where available.
    - **Thread Pools**            : Employing a pool of worker threads to handle I/O operations that can then notify the main thread upon completion.

    The exact method used can vary based on the library's design goals, performance characteristics, and the operating systems it supports.

### Differnce Between Non-Blocking I/O vs AIO.
The main difference between **Non-Blocking I/O with Callbacks** and **Asynchronous I/O (AIO)** lies in the way the I/O operations are scheduled and how the application is notified of their completion.
Here's a comparison table to illustrate the differences:

| Aspect              | Non-Blocking I/O with Callbacks                                                                                              | Asynchronous I/O (AIO)                                                                                                       |
|---------------------|------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------|
| **Execution Model** | Typically uses an event loop that runs in the application's main thread, invoking callbacks when I/O events are ready.       | Uses the kernel to perform I/O operations independently of the application's main execution flow.                            |
| **Notification**    | Callbacks are functions provided by the application that are called by the event loop when I/O is ready.                     | The kernel notifies the application through a mechanism like a callback, signal, or by checking the status of the operation. |
| **Library Support** | Often implemented by libraries that abstract the complexity of non-blocking I/O, such as libuv or Boost.Asio.                | Directly supported by the operating system's kernel, requiring specific AIO system calls.                                    |
| **Control Flow**    | The application retains control over the I/O operations by registering callbacks and handling events as they occur.          | The kernel takes control of the I/O operations, allowing the application to offload the I/O handling.                        |
| **Concurrency**     | Concurrency is managed by the application, which must handle the execution of callbacks without blocking the main thread.    | Concurrency is managed by the kernel, which can handle multiple I/O operations simultaneously.                               |
| **Use Case**        | Suitable for applications that need fine-grained control over I/O operations and can handle them within the main event loop. | Ideal for applications that require high I/O throughput and want to offload the I/O handling to the kernel.                  |
|---------------------|------------------------------------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------|

In summary, **Non-Blocking I/O with Callbacks** is often used in the context of an event-driven architecture where an event loop runs in the application's thread, while **AIO** is a lower-level mechanism where the kernel is responsible for managing the I/O operations.

### Coroutines (C++20):
   - **What**: It's like reading a book and using a bookmark to remember where you stopped.
   - **How**: You write code that can pause at certain points and resume later.
   - **Flow**:
     1. You start a coroutine that may involve I/O.
     2. The coroutine pauses if I/O is not ready, allowing other code to run.
     3. The coroutine resumes when I/O is ready.

  Coroutines in C++20 provide a way to write asynchronous code that can suspend and resume execution at certain points, which is particularly useful for I/O operations.
  Here's how I/O is handled in coroutines and how the switching occurs:

  - **I/O Handling**:
    - When a coroutine reaches an I/O operation that is not ready (e.g., waiting for data to be received on a socket), it can suspend execution using the `co_await` keyword.
    - The `co_await` operator is used in conjunction with an awaitable object, which represents the asynchronous operation.
      The awaitable object knows how to put the coroutine into a suspended state and how to resume it once the I/O is ready.
    - The coroutine does not block the thread; instead, it returns control to the caller, which could be the event loop or another coroutine.

  - **Switching Coroutines**:
    - The coroutine scheduler, which could be part of a library or the language runtime, is responsible for managing the execution of coroutines.
    - When a coroutine suspends, the scheduler determines which coroutine to run next based on its internal logic, which might involve priorities, readiness of I/O, or other factors.
    - Once the I/O operation is ready, the scheduler resumes the suspended coroutine, picking up where it left off.

  Under the hood, the compiler transforms the coroutine into a state machine.
  Each suspension point (`co_await`) is a state transition.
  The state machine is managed by the coroutine's promise object, which handles the lifecycle of the coroutine, including suspension and resumption.

  The actual reading from or writing to I/O is performed using the same system calls or library functions you would use in synchronous code, but they are wrapped in objects that integrate with the coroutine machinery to provide asynchronous behavior.

  In summary, coroutines allow you to write code that looks synchronous but behaves asynchronously, handling I/O operations without blocking the thread and allowing other coroutines to run while waiting for I/O to become ready.
  These techniques allow you to manage I/O operations without blocking your program's execution, enabling it to remain responsive and efficient.

# A comparison of different I/O methods, their typical use cases, efficiency, and scalability:

  | I/O Method                          | Use Cases                                                                                         | Efficiency                                                                                      | Scalability                                                                                                                        |
  |-------------------------------------|---------------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------|
  | **Blocking**                        | Simple scripts, CLI tools, where concurrency isn't critical.                                      | Less efficient as it waits for operations to complete, causing potential idle time.             | Poor scalability due to one operation per thread, leading to high resource usage with many concurrent operations.                  |
  | **Non-Blocking**                    | -                                                                                                 | -                                                                                               | -                                                                                                                                  |
  | (a) Multiplex I/O                   | Servers handling multiple connections, where tasks can wait for I/O without blocking others.      | More efficient than blocking as it can handle multiple I/O operations concurrently.             | Good scalability as one thread can handle multiple operations, but there's overhead in managing the readiness of file descriptors. |
  | (b) Asynchronous I/O (AIO)          | High-performance I/O operations, databases, where operations can be offloaded to the kernel.      | Highly efficient for heavy I/O as it allows other processing to continue while waiting for I/O. | Excellent scalability as the kernel manages I/O operations, allowing for many concurrent operations with less overhead.            |
  | (c) Signal-Driven I/O               | Applications that need to be notified of I/O events but can continue other tasks in the meantime. | Moderate efficiency, as it can continue processing until signaled for I/O readiness.            | Moderate scalability, as each I/O operation requires signal handling, which can become complex with many signals.                  |
  | (d) Non-Blocking I/O with Callbacks | Event-driven applications, GUIs, where responsiveness is key.                                     | Efficient as it allows the application to continue processing and handle I/O when ready.        | Good scalability with an event loop handling callbacks, but callback management can become complex.                                |
  | (e) Coroutines (C++20)              | Applications requiring clean, readable asynchronous code, such as networked services or UIs.      | High efficiency as they allow suspending and resuming operations without blocking threads.      | Excellent scalability as coroutines can be lightweight and managed with less overhead than threads.                                |

  - **Blocking I/O**                                       : is straightforward but can be inefficient and not scalable for high-concurrency scenarios.
  - **Non-Blocking I/O**                                   : methods generally offer better efficiency and scalability, with **AIO** and **Coroutines** being particularly effective for high-performance and highly scalable applications.
  - **Multiplex I/O** and **Signal-Driven I/O**            : provide a middle ground, offering more control and better resource usage than blocking I/O.
  - **Non-Blocking I/O with Callbacks** and **Coroutines** : are modern approaches that provide high efficiency and scalability, especially in complex, event-driven environments.

  The choice of I/O method depends on the specific requirements of the application, such as the need for concurrency, responsiveness, and the complexity of I/O operations.
