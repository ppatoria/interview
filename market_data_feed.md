# Market Data Feed Handling: A Comprehensive Guide

## 1️⃣ Introduction
Market data feed handlers process real-time market data received via **multicast UDP**. Unlike TCP, multicast UDP does not guarantee message order, delivery, or integrity, making it essential to detect and recover lost packets.

### **Challenges in Multicast UDP Handling**
- **No guaranteed delivery:** Lost packets are not retransmitted by the network.
- **Out-of-order arrival:** Messages can arrive in different sequences.
- **Handling burst data:** Sudden spikes in market activity can lead to congestion.
- **Low-latency requirement:** Processing must be efficient with minimal delays.

## 2️⃣ How to Detect Packet Loss in Multicast UDP?
### **(a) Sequence Numbers in Market Data Messages**
Market data messages typically include a **sequence number**, which increments with each message.

### **(b) How Consumers Detect Packet Loss (Gap Detection)**
1. The consumer tracks the last processed sequence number.
2. If the next received message has a **higher sequence number than expected**, a gap is detected.

### **(c) How to Recover Lost Data?**
- **Request retransmission** (if the exchange provides a recovery mechanism).
- **Use redundant feeds** to compare and fill gaps.
- **Infer missing data** when applicable (e.g., previous trade prices).

## 3️⃣ Market Data System Architecture
### **(a) Market Data Feed Handler (UDP Parser)**
- Receives raw multicast UDP packets.
- Parses and structures data for further processing.

### **(b) Market Data Session & Recovery Manager**
- Detects gaps and requests retransmissions.
- Manages session state and failover handling.

💡 **This article will focus on the Market Data Feed Handler (UDP Parser).**

## 4️⃣ Example Structure of a Multicast UDP Packet
### **(a) Breaking Down the Packet**
| Field | Size | Description |
|--------|------|-------------|
| UDP Header | 8 bytes | Source/Destination Port, Length, Checksum |
| Message Header | Variable | Sequence Number, Message Count, Flags |
| Message Body | Variable | Market data content |

### **(b) Examples**
#### **1 Message Packet**
```
| UDP Header | MsgHeader (Seq:1001, Count:1) | Msg1 |
```
#### **2 Message Packet**
```
| UDP Header | MsgHeader (Seq:1002, Count:2) | Msg2 | Msg3 |
```
#### **1 Complete + 1 Partial Message**
```
| UDP Header | MsgHeader (Seq:1004, Count:2) | Msg4 (Complete) | Msg5 (Partial) |
```
### **(c) Example Binary Data**
```
4500003c1c4640004011b1e6c0a80102c0a80101
...
```
_(Each byte represents part of the packet, broken down in detail.)_

## 5️⃣ Steps Before Parsing a Market Data Message
### **(a) Check Endianness**
Some exchanges use **big-endian**, while most CPUs use **little-endian**.

### **(b) Convert Multi-Byte Fields**
Example:
```cpp
uint32_t seq_num = ntohl(header.seq_num);
```

## 6️⃣ Parsing Market Data
### **(a) Zero-Copy Parsing**
Using pointers to access raw packet data **without copying**:
```cpp
struct MsgHeader { uint32_t seq; uint16_t count; };
const MsgHeader* hdr = reinterpret_cast<const MsgHeader*>(packet);
```
### **(b) Safe Copy Using Modern C++**
```cpp
MsgHeader hdr;
std::bit_copy(packet, packet + sizeof(MsgHeader), &hdr);
```
✔️ **Pros:** Safer, avoids unaligned memory access.

## 7️⃣ Alignment vs No Padding
### **(a) Tradeoff**
- **Aligned structs:** Faster access but **may waste memory** due to padding.
- **Packed structs:** Save memory but **may cause unaligned access**.

### **(b) Using Packed Struct for Parsing, Aligned Struct for Processing**
```cpp
#pragma pack(push, 1)
struct PackedMsg { uint32_t seq; uint16_t count; };
#pragma pack(pop)
```
```cpp
AlignedMsg msg = std::bit_cast<AlignedMsg>(packed_msg);
```
✔️ **Zero-copy alternative using `std::bit_cast`** avoids unaligned access.

## 8️⃣ Handling Partial Messages
### **(a) What Are Partial Messages?**
A message split across multiple UDP packets.

### **(b) How Do Partial Messages Look?**
```
| Packet 1: Msg1001 (Complete) | Msg1002 (Partial) |
| Packet 2: Msg1002 (Remaining) | Msg1003 (Complete) |
```
### **(c) Identifying Partial Messages**
- Check **message length vs. packet size**.

### **(d) Handling Partial Messages**
- Store partial fragments in a **buffer**.
- Reassemble when all fragments arrive.

### **(e) Extra Data Structures Used**
- **Hashmap** (for quick lookup).
- **Linked list** (to track fragments).

## 9️⃣ Reassembly Strategies for Partial Messages
### **(a) Simple Buffering**
✅ **Pros:** Easy to implement. ❌ **Cons:** Inefficient for out-of-order arrivals.

### **(b) Linked Fragment Storage**
✅ **Pros:** Saves memory. ❌ **Cons:** More complex pointer management.

### **(c) Gap-Filling with Sequence Number Tracking**
✅ **Pros:** Efficient for in-order processing. ❌ **Cons:** Needs extra tracking logic.

### **(d) Sliding Window Approach**
✅ **Pros:** Handles out-of-order packets. ❌ **Cons:** May drop valid messages if delays exceed window.

### **(e) Deferred Processing Queue**
✅ **Pros:** Ensures ordered processing. ❌ **Cons:** Extra latency waiting for timeout.

## 🔥 Comparison Table
| Method | Efficient? | Handles Gaps? | Maintains Order? | Overhead |
|--------|-----------|--------------|----------------|----------|
| Simple Buffering | ❌ No | ❌ No | ✅ Yes | Low |
| Linked Storage | ✅ Yes | ✅ Yes | ✅ Yes | Medium |
| Hashmap | ✅ Yes | ✅ Yes | ✅ Yes | Medium |
| Sliding Window | ✅ Yes | ✅ Yes (Limited) | ❌ No | Low |
| Deferred Queue | ✅ Yes | ✅ Yes | ✅ Yes | High |

## Conclusion
This guide covers **market data feed handling over multicast UDP**, including:
- Detecting and handling **packet loss**
- **Parsing** strategies with memory efficiency
- **Reassembly techniques** for partial messages

Each technique has trade-offs based on **latency, memory usage, and order guarantees**. Choosing the right approach depends on **market requirements** and **performance constraints**. 🚀

