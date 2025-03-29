# Queue using single linked list
### ** Algorithm for a Singly Linked List with Both Head and Tail**

---

## **Add a Node (`add(data)`)**
### **Scenarios & Solutions:**
1. **List is empty (`head == nullptr` and `tail == nullptr`)**  
   - Create a new node.
   - Set both `head` and `tail` to point to this new node.

2. **List is not empty**  
   - Create a new node.
   - Link the current `tail` to the new node (`tail->next = new node`).
   - Update `tail` to point to the new node.

---

## ** Remove a Node (`remove(data)`)**
### **Scenarios & Solutions:**
1. **List is empty (`head == nullptr`)**  
   - Do nothing and return `false` (no elements to remove).

2. **Node to remove is the `head` node**  
   - Move `head` to `head->next`.
   - If the new `head` is `nullptr`, update `tail` to `nullptr` as well.
   - Delete the old head node.

3. **Node to remove is in the middle of the list**  
   - Traverse the list while keeping track of the previous node.
   - If a match is found, adjust the previous node’s `next` pointer to skip the node being removed.
   - Delete the removed node.

4. **Node to remove is the `tail` node**  
   - Traverse the list to find the second last node.
   - Update the second last node’s `next` pointer to `nullptr`.
   - Update `tail` to point to this second last node.
   - Delete the old tail node.

5. **Node not found**  
   - Return `false` (nothing was removed).

---

## **Find a Node (`find(data)`)**
### **Scenarios & Solutions:**
1. **List is empty (`head == nullptr`)**  
   - Return `nullptr` (nothing to find).

2. **Search for `data` in the list**  
   - Start from `head` and traverse each node.
   - If a node with `data` is found, return a pointer to it.
   - If the end of the list is reached, return `nullptr` (not found).

---

## **Print the List (`print()`)**
### **Scenarios & Solutions:**
1. **List is empty (`head == nullptr`)**  
   - Print "List is empty."

2. **List is not empty**  
   - Start from `head` and traverse each node.
   - Print the `data` of each node.
   - Stop when `nullptr` is reached.

---

### **Summary**
- **`add(data)`** → Handle empty list, then add after `tail`.  
- **`remove(data)`** → Handle head, middle, and tail cases separately.  
- **`find(data)`** → Traverse from `head` to find the node.  
- **`print()`** → Print each node’s data from `head` to `tail`.  

