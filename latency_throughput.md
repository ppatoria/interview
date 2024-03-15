# Units for measuring Latency and Throughput.
- **1 second** is equal to **1000 milliseconds**. So, if you have a time interval of 1 millisecond, it is 0.001 seconds.
- **1 second** is equal to **1,000,000 microseconds**. So, if you have a time interval of 1 microsecond, it is 0.000001 seconds.
- **1 millisecond (ms)** is equal to **1000 microseconds (µs)**. So, if you have a time interval of 1 microsecond, it is 0.001 milliseconds. Conversely, 1 millisecond is 1000 times longer than 1 microsecond.
- **1 microsecond** is equal to **1000 nanoseconds (ns)**.

So, if you have a time interval of 1 nanosecond, it is 0.001 microseconds, or 0.000001 milliseconds, or 0.000000001 seconds. Conversely, 1 second is 1,000,000,000 times longer than 1 nanosecond. These units of time are often used in contexts where precise measurements are required, such as in scientific experiments, computer processing times, and high-frequency trading.
# Latency and Throughput in context of trading applications:

## Latency:
It measures the time it takes for data to get from one designated point to another.
In trading applications, latency can be defined as the time interval between any two points in a trading infrastructure.
For example, the tick-to-trade latency is the time difference between the order reaching the exchange and the market data that triggers it.
Lower latency results in more responsive applications and better interactivity.

## Throughput:
It measures the actual rate of successful data transfer.
In trading applications, throughput could refer to the number of trades executed per unit of time or the number of market data updates processed per unit of time.
Higher throughput allows faster data transfers.

It's important to note that latency and throughput have an inverse relationship – higher latency negatively impacts throughput.
However, higher bandwidth does not directly reduce latency or guarantee higher throughput.
Applications may require balancing tradeoffs between latency, throughput, and bandwidth based on their usage.

# Low Latency:
When we say that 1 millisecond (ms) is considered low latency in the context of trading applications, we're referring to the total time it takes for a system to respond to an event or request.
This includes the time it takes to:

1. Receive market data from the exchange (over the network).
2. Process the received data (in the trading application).
3. Make a decision based on the processed data (using a trading algorithm).
4. Generate an order based on that decision.
5. Send the generated order back to the exchange (over the network).

All these steps together should ideally take less than 1 ms for a system to be considered low latency.
The exact time for each step can vary depending on various factors such as network speed, hardware performance, and the efficiency of the software.

It's important to note that achieving such low latencies requires significant technical expertise and infrastructure investment.
High-frequency trading (HFT) firms, for example, often invest heavily in cutting-edge technology and infrastructure to minimize latency as much as possible.
# Mid level latency:
In the context of trading applications, "mid-level latency" isn't a standard term, but it could be used to describe latencies that are neither extremely low (like those sought by high-frequency trading firms) nor high.

While there's no universal benchmark to define what constitutes "low", "mid", or "high" latency, some firms may consider latencies of under 1 millisecond as low latency.
In contrast, a medium to low frequency trading strategy might consider latencies around 14.8 milliseconds as acceptable.

It's important to note that the acceptable level of latency can vary widely depending on the specific trading strategy, market conditions, and the resources available to the trading firm.

In a latency-sensitive environment like trading, even small amounts of latency can have a significant impact.
For example, latency affects the time between the initiation of a transaction and its execution.
In fast-moving markets, lower latency means a higher chance of executing an order at the displayed price before it changes.

| Low Latency      | Mid Level Latency    |
| 1ms              | 14.8 ms              |
| High Throghput   | Mid Level Throughput |
| 12000 per second | 800 per second       |

# The throughput in low latency trading applications:
I can vary widely depending on the specific trading strategy, market conditions, and the resources available to the trading firm.
Throughput refers to the number of messages being received, sent, and processed by the system and is usually measured in updates per second.
For example, one simple low-latency trading system was able to achieve a maximum throughput of 12,000 orders per second.

However, as the message rate increases, so do the latency figures.
Therefore, achieving high throughput while maintaining low latency can be a challenging task and requires significant technical expertise and infrastructure investment.
## Example that processes market data updates and sends out trading orders.

1. **Low message rate, low latency**: Let's say the system is receiving 1,000 market data updates per second, and it's able to process each update and potentially send out a trading order within 1 microsecond.
   In this scenario, the system has both low latency (1 microsecond) and high throughput (1,000 orders per second).

2. **High message rate, increased latency**: Now, let's say the rate of market data updates increases to 1,000,000 per second.
   The system might start to struggle with this higher load.
   It might take longer to process each update, say 10 ms instead of 1.
   And it might not be able to keep up with all the updates, so it might only be able to send out 50 orders per second instead of 1,000.
   In this scenario, the increased message rate has led to increased latency (10 ms) and decreased throughput (50 orders per second).

Achieving high throughput while maintaining low latency in the face of high message rates can be challenging.
It requires efficient algorithms, fast networks, and powerful hardware.
For example, the system might need to use parallel processing to handle multiple market data updates simultaneously, or it might need to use a faster network to send out orders more quickly.

# Ultra-low latency:
It refers to a computer network that is optimized to process a very high volume of data packets with an extraordinarily low tolerance for delay.
In the context of trading applications, ultra-low latency is almost exclusively used in capital markets firms to support high-frequency trading systems.
Ultra-low latency is measured in nanoseconds, while low latency is measured in milliseconds.
Therefore, ultra-low latency delivers a response much faster, with fewer delays than low latency.
Achieving ultra-low latency requires significant technical expertise and infrastructure investment.
It's particularly important in scenarios where every microsecond counts, such as high-frequency trading.
Ultra-low latency in the context of High-Frequency Trading (HFT) refers to the extremely short time interval for a trading system to respond to market events.
This latency is often measured in microseconds (µs), or even nanoseconds (ns), and achieving such low latencies is a key competitive advantage for HFT firms.
While there isn't a universally agreed-upon standard for what constitutes "ultra-low" latency, some HFT firms aim for single-digit microsecond or even sub-microsecond level latency.
This includes the entire round-trip time for receiving a market data update, making a trading decision based on that update, and sending a trading order to the exchange.
However, achieving such ultra-low latencies requires significant technical expertise and infrastructure investment.
