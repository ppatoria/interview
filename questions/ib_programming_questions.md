# Set 1
Here are some common programming interview questions that investment banks often ask:

6. **What is the Singleton design pattern and how do you implement it in c++23?**
1. **Reverse a String:** Write a function to reverse a given string.
4. **Palindrome Check:** Write a function to check if a given string is a palindrome.

```cpp
#include <iostream>
#include <string>
#include <cctype> // For std::tolower

bool isPalindrome(const std::string& str) {
    int n = str.length();
    int mid = n / 2; // Calculate the midpoint of the string

    for (int i = 0; i < mid; ++i) {
        if (std::tolower(str[i]) != std::tolower(str[n - i - 1])) {
            return false; // If mismatch found
        }
    }

    return true; // All characters matched
}
```

5. **Anagram Check:** Write a function to check if two strings are anagrams of each other.
6. **Bubble Sort:** Implement the bubble sort algorithm.
7. **Merge Sort:** Implement the merge sort algorithm.
8. **Factorial Calculation:** Write a function to calculate the factorial of a number.
9. **Non-Repeating Characters:** Write a function to find the first non-repeating character in a string.
10. **Substring Replacement:** Write a function to replace a substring in a string with another substring.

1. **Order Book Implementation:** Write a function to implement an order book that can handle buy and sell orders.
2. **Trade Matching Algorithm:** Write a function to match buy and sell orders based on price and time priority.
3. **Pricing Model:** Implement a simple pricing model for a financial instrument (e.g., Black-Scholes model for options pricing).
4. **Risk Management:** Write a function to calculate Value at Risk (VaR) for a portfolio of assets.
5. **Monte Carlo Simulation:** Implement a Monte Carlo simulation to estimate the future price of a stock.
6. **Market Making Strategy:** Write a function to simulate a market-making strategy that buys at the bid price and sells at the ask price.
7. **Algorithmic Trading Strategy:** Implement a basic algorithmic trading strategy (e.g., moving average crossover).
8. **Slippage Calculation:** Write a function to calculate slippage for a given trade size and market conditions.
9. **Portfolio Optimization:** Implement a function to optimize a portfolio based on expected returns and risk (e.g., using the Markowitz model).
10. **Backtesting Framework:** Write a function to backtest a trading strategy using historical data.


1. **Implement a function to match buy and sell orders in an order book.**
2. **Write a function to calculate the Value at Risk (VaR) for a portfolio of assets.**
3. **Implement a Monte Carlo simulation to estimate the future price of a stock.**
4. **Design a basic algorithmic trading strategy using moving averages.**
5. **Write a function to optimize a portfolio based on expected returns and risk.**

These examples are inspired by real questions and reflect the types of problems you might encounter. For more specific and up-to-date questions, you might want to check resources like **Liquidity Shark** or **Internshala** which provide a collection of real interview questions from candidates who have gone through the process.

# Set 2
Here’s a categorized list of programming interview questions commonly asked in investment banking interviews, specifically tailored for roles involving low-latency systems, algorithmic trading, and financial applications:

---

### **General Programming Questions**
1. Write code to implement a **LRU Cache**.
2. Implement a **thread-safe Singleton pattern** in C++.
3. Write a program to **find the first non-repeating character** in a string.
4. Reverse a linked list in-place.
5. Implement a program to check if a binary tree is balanced.

---

### **Data Structures**
1. Design a data structure to maintain a list of stock prices and efficiently provide the:
   - Maximum price in the last `k` days.
   - Minimum price in the last `k` days.
2. Implement an **order book** data structure with buy/sell orders and matching logic.
3. Write a program to find the **kth largest element** in an unsorted array.
4. Implement a **priority queue** using a binary heap.
5. Design a **trie** to store financial instrument identifiers.

---

### **Algorithms**
1. Implement **quick sort** and explain its time complexity.
2. Write code to find all pairs of integers in an array that sum up to a given value.
3. Optimize the execution of multiple trading strategies by minimizing latency.
4. Write a program to **find the shortest path** in a weighted graph (Dijkstra's algorithm).
5. Given a list of trades, determine if it is possible to close all positions to zero.

---

### **Concurrency and Multithreading**
1. Write a program to implement a **thread pool** in C++ or Python.
2. Solve the **Dining Philosophers Problem**.
3. Demonstrate how to handle **race conditions** in a multi-threaded environment.
4. Implement a **message-passing system** between two threads using a queue.
5. Write code to simulate a producer-consumer system with synchronization.

---

### **Low-Latency Systems**
1. Implement a **custom memory allocator** in C++.
2. Write a program to parse and process large market data feeds

 with minimal latency.
3. Optimize a program that reads market orders from a file and processes them in real-time.

While I don't have access to exact, previously asked questions from specific investment bank interviews, I can provide a list of **realistic problems inspired by frequently asked questions** in coding interviews for trading roles. These examples focus on challenges typical of live problem-solving sessions in trading system contexts:

---

### **Realistic Trading-Related Coding Questions**

1. **Order Matching Engine**
   - Implement a simplified **order matching engine** for a stock exchange. 
   - Input: A series of buy/sell orders with price and quantity.
   - Output: Matched trades and remaining orders in the order book.
   - Constraints: Handle orders in a FIFO manner and support multiple price levels.

2. **Market Data Aggregator**
   - Write a program to process market data streams from multiple sources, aggregate the data, and publish the best bid/offer for each financial instrument.
   - Input: Market data updates (price, quantity, timestamp).
   - Output: Real-time best bid and offer for each instrument.
   - Challenge: Ensure low-latency updates and maintain data consistency.

3. **Trade PnL Calculation**
   - Calculate the **Profit and Loss (PnL)** for a given set of trades.
   - Input: List of executed trades (instrument, side, price, quantity) and end-of-day market prices.
   - Output: Total realized and unrealized PnL.
   - Consider edge cases like short selling and partial fills.

4. **Latency Optimization**
   - Optimize the processing of a high-frequency trade feed by reducing the latency of parsing and storing trades in a database.
   - Input: Stream of trade messages in FIX format.
   - Output: Parsed trades stored in a memory-efficient data structure.
   - Challenge: Minimize memory allocation and reduce string parsing overhead.

5. **VWAP Implementation**
   - Write a program to calculate the **Volume Weighted Average Price (VWAP)** of trades for a specific stock.
   - Input: List of trades (price, volume).
   - Output: Real-time VWAP as new trades are added.
   - Bonus: Support multiple stocks and maintain VWAP calculations simultaneously.

6. **Order Splitting Algorithm**
   - Implement an algorithm to split a large parent order into smaller child orders to minimize market impact.
   - Input: Total quantity, time horizon, and market liquidity.
   - Output: Sequence of smaller orders with timestamps.
   - Challenge: Adjust order sizes dynamically based on market conditions.

7. **Dark Pool Matching**
   - Simulate a **dark pool** where trades are matched without exposing order details.
   - Input: Anonymous buy/sell orders with price and quantity.
   - Output: Matched trades and remaining unmatched orders.
   - Constraint: Trades can only match at overlapping prices.

8. **Arbitrage Detection**
   - Detect arbitrage opportunities given a list of currency exchange rates.
   - Input: Matrix of exchange rates between currencies.
   - Output: Sequence of trades to exploit the arbitrage.
   - Approach: Use graph algorithms like Bellman-Ford to find negative weight cycles.

9. **Order Book Snapshot**
   - Write a function to generate a **snapshot of the order book** at any given moment.
   - Input: List of order updates (add, modify, cancel) and a timestamp.
   - Output: Current state of the order book (bids and asks).
   - Bonus: Optimize for memory usage when handling millions of updates.

10. **Stop Loss Order Simulation**
    - Implement a system to execute **stop loss orders** when a stock price crosses a predefined threshold.
    - Input: Real-time price updates and stop loss orders.
    - Output: Execution of stop loss orders at the crossing price.
    - Challenge: Ensure no order is missed during rapid price changes.

---

### **Live-Coding Scenarios**

1. **High-Frequency Trading Analysis**
   - Given a stream of executed trades, calculate the **average trade size** and the **most traded instrument** in real-time.
   - Use data structures like hashmaps for quick lookups.

2. **Exchange Connectivity Simulator**
   - Write a program to simulate **order submission and acknowledgment** to/from an exchange.
   - Input: Series of order messages.
   - Output: Simulated exchange responses (acknowledgments, fills).

3. **Risk Management System**
   - Build a system to monitor **exposure limits** for a portfolio of trades.
   - Input: List of trades and position limits per instrument.
   - Output: Alerts for limit breaches.

---

These questions are highly specific to trading systems and focus on the skills tested during live coding rounds for trading roles in investment banks. Would you like detailed solutions or mock scenarios for any of these?
4. Implement a lock-free stack using atomic operations.
5. Write a program to measure and reduce cache misses in a data-intensive application.

---

### **Financial Algorithms**
1. Implement an algorithm to calculate **VWAP (Volume Weighted Average Price)**.
2. Write a program to generate **synthetic market data** for testing trading algorithms.
3. Design a module to compute real-time **PnL (Profit and Loss)** for a trading desk.
4. Implement logic for **order splitting** and **execution** across multiple venues.
5. Write an algorithm to detect **arbitrage opportunities** in a given set of currency exchange rates.

---

### **C++/Rust Specific Questions**
1. Explain the differences between `std::vector` and `std::list`. When would you use one over the other?
2. Write a program in C++ using **RAII** to manage resources.
3. Implement a **move constructor and move assignment operator** for a custom class.
4. Write a program in Rust to demonstrate **ownership and borrowing** in a trading scenario.
5. Compare the performance of a C++ and Rust implementation for parsing large JSON files.

---

### **Optimization and System Design**
1. Design a **low-latency market data system**.
2. Optimize the latency of an application processing high-frequency trades.
3. Write a program to simulate a **smart order routing** mechanism.
4. Design an architecture to support **real-time risk calculation**.
5. Implement and optimize a **garbage collector** for a custom scripting language.

---

### **Mathematical and Logical Puzzles**
1. Given a large set of numbers, how would you efficiently find the **median**?
2. Write code to solve the **N-Queens problem**.
3. Implement a **Monte Carlo simulation** for option pricing.
4. Find the longest palindromic substring in a given string.
5. Solve a **sudoku puzzle** using backtracking.

---

### **Domain-Specific Knowledge**
1. Write a program to calculate the **Greeks** for a given financial option.
2. Implement an **algorithm for order matching** in a stock exchange.
3. Simulate a **limit order book** and implement FIFO matching.
4. Design a **circuit breaker** for trading systems.
5. Implement a program to compute **Sharpe Ratio** and **Sortino Ratio** for a trading strategy.

---

# Set 3
Investment banks often ask programming questions to assess a candidate's problem-solving, analytical, and coding skills. Here are some common types of questions you might encounter:

**Data Structures and Algorithms:**

* **Array/List Manipulation:**
    * Reversing an array
    * Finding the maximum/minimum element
    * Implementing a stack or queue
    * Sorting algorithms (e.g., bubble sort, merge sort, quick sort)
    * Searching algorithms (e.g., binary search)
* **String Manipulation:**
    * Checking for palindromes
    * Finding the longest common substring
    * Implementing string reversal
    * Counting the frequency of characters
* **Trees and Graphs:**
    * Tree traversal (e.g., depth-first search, breadth-first search)
    * Finding the shortest path in a graph
    * Detecting cycles in a graph

**Financial-Specific Problems:**

* **Valuation:**
    * Calculating present value or future value of an investment
    * Implementing a discounted cash flow (DCF) model
* **Portfolio Optimization:**
    * Finding the optimal asset allocation in a portfolio
* **Risk Management:**
    * Calculating Value at Risk (VaR) or other risk metrics
* **Trading Strategies:**
    * Implementing simple trading algorithms (e.g., moving average crossover)

**Coding Challenges:**

* **Algorithmic Trading:**
    * Designing a high-frequency trading algorithm
    * Implementing a market maker strategy
* **Risk Modeling:**
    * Building a model to predict credit risk or market risk
* **Financial Data Analysis:**
    * Cleaning and processing financial data
    * Performing statistical analysis on financial time series

**Tips for Preparing:**

* **Practice, Practice, Practice:** The best way to prepare is to solve coding problems regularly. Use online platforms like LeetCode, HackerRank, or Codewars.
* **Learn Data Structures and Algorithms:** Have a strong foundation in data structures and algorithms.
* **Understand Financial Concepts:** Familiarize yourself with basic financial concepts and formulas.
* **Explain Your Thought Process:** Clearly communicate your approach to solving a problem.
* **Write Clean and Efficient Code:** Focus on writing readable and well-structured code.
* **Ask Questions:** If you're unsure about a question, don't hesitate to ask clarifying questions.

Remember, the specific questions you encounter will vary depending on the bank, the role you're applying for, and the interviewer's preferences. However, by preparing for these common types of questions, you can increase your chances of success in your investment banking interviews.
While it's not possible to predict the exact questions you'll be asked in a live coding interview, here are some common investment bank trading-related problems you can practice to prepare:

**Market Order Execution:**

* Simulate the execution of a market order for a given stock, considering factors like order size, available liquidity, and market impact.
* This might involve calculating the成交量 (chéng jiāo liàng,成交量) - volume traded and 平均成交价格 (píng jūn chéng jiāo jià gé) - average execution price  based on a simulated order book or market depth data.

**Order Book Analysis:**

* Write a program to analyze an order book (a list of buy and sell orders for a security) and identify potential trading opportunities.
* This could involve calculating the bid-ask spread, identifying imbalances between buy and sell orders, or finding arbitrage opportunities.

**Algorithmic Trading Strategies:**

* Implement a simple algorithmic trading strategy, such as a moving average crossover or a relative strength index (RSI) strategy.
* This would involve fetching historical price data, calculating the indicators, and generating buy/sell signals based on the strategy's rules.

**Risk Management Calculations:**

* Write a program to calculate Value at Risk (VaR) for a portfolio of assets.
* VaR is a metric used to estimate the potential loss of a portfolio over a given time horizon with a specific confidence level.

Here are some additional tips for preparing for live coding interviews:

* **Practice with online coding platforms:** There are many online platforms like LeetCode, HackerRank, and Codewars that offer practice problems for coding interviews.
* **Focus on code clarity and efficiency:** Make sure your code is well-commented, easy to read, and avoids unnecessary complexity.
* **Test your code thoroughly:** Before the interview, make sure you can test your code with different inputs and ensure it produces the correct results.
* **Be prepared to explain your thought process:** During the interview, be able to explain your approach to solving the problem and why you made certain coding decisions.

By practicing these problems and following these tips, you can increase your chances of success in your investment banking trading interviews.

Remember, these are just examples, and the specific questions you'll be asked willvary. However, by understanding the types of problems that are commonly asked and by practicing your coding skills, you can be well-prepared for your interview.
 
