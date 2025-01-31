# **Understanding False Sharing in Multi-Core Architectures**  

### **What is False Sharing?**  
False sharing occurs when multiple threads modify independent variables that reside in the same **cache line**, leading to unnecessary cache invalidation and performance degradation.  

### **Why is it Called 'False' Sharing?**  
- **What is shared?** A cache line containing different variables used by multiple threads.  
- **Why 'false'?** The variables are **not logically shared**, but since they occupy the same cache line, any modification by one thread forces an update across cores, causing inefficiencies.  

### **When Does False Sharing Happen?**  
False sharing occurs in **multi-threaded programs** where:  
- Threads modify different variables stored adjacently in memory.  
- CPUs use private L1/L2 caches but share an L3 cache.  
- **Cache coherence protocols** (e.g., MESI) cause unnecessary invalidations when one core modifies a cache line used by another.  

This leads to **performance degradation** despite the variables being independent.  

