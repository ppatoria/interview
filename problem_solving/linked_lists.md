# Create Binary Search Tree.

## Algorithm for Creating a Binary Search Tree (BST)

  **Data Structure:**

  * A Binary Search Tree (BST) is a tree data structure where each node contains a key value.
  * The key values follow a specific order:
      * Every node's left child has a key value **less than** the parent node's key.
      * Every node's right child has a key value **greater than** the parent node's key.

  **Operations:**

  1. **Initialization:** Create an empty tree (a null pointer).

  2. **Insertion:**
    * **Base Case:** If the tree is empty (root is null), create a new node with the key value to be inserted and set it as the root.
    * **Recursive Case:**
      * Compare the key value to be inserted with the current node's key:
          * If the key to be inserted is **less than** the current node's key, recursively call the insertion function on the left subtree.
          * If the key to be inserted is **greater than** the current node's key, recursively call the insertion function on the right subtree.
      * After the recursive call, the returned subtree becomes the appropriate child of the current node.

## Implementation:
  ```
  #include <iostream>
  #include <memory>

  using namespace std;

  template<typename T>
  class BST{
      private:
          struct node{
              node* left;
              node* right;
              T data;

          node(const T& _data) : data(_data), left(nullptr), right(nullptr)
          {}

          };

          node* head;

      private:

          bool isEmpty(){
              return head == nullptr;
          }

          node* insert(node*& root, const T& data){
              if(root == nullptr){
                  root = new node(data);
                  return root;
              }

              if(root->data == data)
                  return root;

              if(data < root->data)
                  return insert(root->left, data);
              else
                  return insert(root->right, data);
          }

          node* search(node* root, const T& data) const {
              if (root == nullptr)  return nullptr;

              if (root->data == data)
                  return root;

              if (data < root->data)
                  return search(root->left, data);
              else
                  return search(root->right, data);
          }


          void inorder(node* root){
              if (root == nullptr) return;

              inorder(root->left);
              cout << root->data << endl;
              inorder(root->right);
          }
      public:
          node* insert(const T& data){
              return insert(head, data);
          }

          node* search(const T& data){
              return search(head, data);
          }

          void inorder(){
              inorder(head);
          }

  };

  int main(){
      BST<int> bst;
      bst.insert(1);
      bst.insert(2);
      bst.insert(3);
      bst.insert(4);
      bst.insert(5);

      bst.inorder();
      return 0;
  }
  ```

## Time and space complexity:
  The time and space complexity of insertion, deletion, and search operations in a Binary Search Tree (BST) depend on the height of the tree, which can vary depending on the order of insertions. 
  Here's a breakdown of the complexities for each case:

  **Time Complexity:**

  | Operation | Best Case | Average Case | Worst Case |
  |---|---|---|---|
  | Search | O(log n) | O(log n) | O(n) |
  | Insertion | O(log n) | O(log n) | O(n) |
  | Deletion | O(log n) | O(log n) | O(n) |

  **Explanation:**

  * **Best Case (O(log n)):** This occurs when the BST is perfectly balanced, meaning the height of the tree is minimal for the number of nodes (e.g., a complete binary tree).
      In this case, all search, insertion, and deletion operations involve traversing a path down to the desired node in logarithmic time with respect to the number of nodes (n).
  * **Average Case (O(log n)):** Assuming random insertion of elements, the BST tends to be somewhat balanced on average.
      Search, insertion, and deletion operations typically involve traversing a path with a height proportional to the average height of a balanced BST, which is also logarithmic in the number of nodes (O(log n)).
  * **Worst Case (O(n)):** This happens when the BST becomes unbalanced, resembling a linked list.
      In the worst case, all nodes may be strung together in a single path, resulting in a height of n.
      Search, insertion, and deletion operations in this scenario devolve to linear time complexity (O(n)) as they essentially traverse the entire list.

  **Space Complexity:**

  | Operation | Space Complexity |
  |---|---|
  | Search | O(log n) |
  | Insertion | O(log n) |
  | Deletion | O(log n) |

  **Explanation:**

  The space complexity for all operations in a BST is generally considered O(log n) in the best and average cases.
  This is because the recursive calls during search, insertion, and deletion involve a constant amount of extra space on the call stack for storing temporary variables and function pointers.
  However, in the worst-case scenario with an unbalanced tree, the call stack depth can grow linearly with the height (n), leading to a space complexity of O(n) in rare cases.

  **Additional Notes:**

  - Balanced BSTs like AVL trees and Red-Black trees guarantee O(log n) time complexity for search, insertion, and deletion in all cases due to self-balancing mechanisms.
  - The actual space complexity might be slightly higher than O(log n) due to additional factors like node overhead (storing data and pointers).

  In summary, for a well-balanced BST, you can expect logarithmic time complexity (O(log n)) for search, insertion, and deletion operations, making it a very efficient data structure for sorted data.
  However, the importance of maintaining balance or using self-balancing BST variants is crucial to avoid potential performance degradation in the worst case.
# How would you detect a cycle in a linked list?

## Detect cycle but doesn't locate the beginning of the cycle:

  **Floyd's Cycle-Finding Algorithm (also known as the Tortoise and Hare Algorithm):**

  **Problem:**

  Given a linked list (potentially containing a cycle), this algorithm efficiently determines whether the list has a cycle
  and, if so, doesn't locate the beginning of the cycle (although modifications exist to do that as well).

  **Intuition:**

  Imagine two runners (Tortoise and Hare) racing on a circular track. The Tortoise moves slowly, taking one step at a time, while the Hare moves twice as fast, taking two steps at a time.

  - **No Cycle:** If there's no cycle, the Hare will eventually reach the end of the list (null pointer) before encountering the Tortoise.
  - **Cycle Present:** If there's a cycle, the Hare and Tortoise will eventually meet at some point within the cycle, regardless of their starting positions.

  **Algorithm Steps:**

  1. **Initialization:**
    - Create two pointers, `slow` and `fast`, both initially pointing to the head of the linked list.

  2. **Iteration:**
    - While both pointers are not null:
      - Move the `slow` pointer one step forward (slow = slow->next).
      - Move the `fast` pointer two steps forward (fast = fast->next->next, assuming two valid next pointers exist).

  3. **Cycle Detection:**
    - If `slow` and `fast` ever become equal, it indicates a cycle exists in the linked list.
    - If `fast` becomes null (reaches the end of the list) before `slow` and `fast` meet, there's no cycle.

  **Explanation:**

  - **No Cycle:** If there's no cycle, the `fast` pointer will gain distance on the `slow` pointer every iteration. Eventually, `fast` will reach the end of the list (null pointer), terminating the loop.
  - **Cycle Present:** In a cycle, the `fast` pointer "laps" the `slow` pointer within the cycle. Since the distance covered by the `fast` pointer in each iteration is a multiple of the `slow` pointer's distance, they are bound to meet at some point within the cycle.

## Space and Time Complexity:
  **Time Complexity:**
  - Best case (no cycle): O(n), where n is the number of nodes in the list (fast pointer reaches the end in linear time).
  - Worst case (cycle present): O(c), where c is the length of the cycle (both pointers meet within the cycle in a constant number of steps after entering it).

  **Memory Complexity:**
  - O(1), as it only uses constant extra space for the two pointers.

  **Advantages:**
  - Efficient for cycle detection.
  - Simple to understand and implement.
  - Low memory usage.

  **Disadvantages:**
  - Doesn't directly locate the beginning of the cycle (although modifications exist).
  - Not suitable for other linked list operations (like finding the nth node).

  **Applications:**
  - Detecting infinite loops in programs.
  - Validating file systems (checking for circular references).
  - Network routing loop detection.

## Algorithm modification to Floyd's Cycle-Finding Algorithm to locate the beginning of the cycle in a linked list:

  **Steps:**

  1. **Perform Cycle Detection:** Use the original Floyd's Cycle-Finding Algorithm described previously. This will confirm the presence of a cycle and ensure `slow` and `fast` meet at some point within the cycle.

  2. **Move `slow` to the Head:** Move the `slow` pointer back to the head of the linked list.

  3. **Iterate and Find Intersection:**
    - Maintain the `fast` pointer at the meeting point from step 1.
    - While `slow` and `fast` are not equal:
      - Move both `slow` and `fast` one step forward in each iteration (`slow = slow->next`, `fast = fast->next`).
    - Once `slow` and `fast` become equal again, the node they point to is the beginning of the cycle.

  **Explanation:**

  1. After Floyd's Cycle-Finding Algorithm confirms a cycle, `slow` is somewhere within the cycle, and `fast` is some number of steps ahead of `slow` within the same cycle.
  2. By moving `slow` back to the head, we create two "runners" again:
    - `slow`: Starts at the beginning and moves one step at a time.
    - `fast`: Remains at a point within the cycle.
  3. Since both `slow` and `fast` are now within the cycle, they will eventually "collide" again at the exact same node, which is the beginning of the cycle. This is because for every step `slow` takes to traverse the list from the beginning, `fast` takes one step to "catch up" within the cycle.

  **Time Complexity:**

  - The additional steps after cycle detection add O(c) time complexity, where c is the length of the cycle. This is because both `slow` and `fast` need to traverse the cycle once to meet at the beginning.
  - Overall complexity becomes O(n + c), where n is the number of nodes in the list (worst case).

  **Space Complexity:**

  - Remains O(1) as it still only uses constant extra space for the two pointers.

  **Note:**

  This modification assumes you have already implemented the basic Floyd's Cycle-Finding Algorithm. You can integrate these additional steps after confirming the presence of a cycle.
# Implement a function to partition a linked list around a value `x`, such that all nodes less than `x` come before all nodes greater than or equal to `x`.

## Algorithm:

  1. **Initialize:**
      - Two empty linked lists : `left` and `right` to store nodes less than and greater than/equal to the partition value `x` respectively.
      - Two pointers           : `ltail` pointing to the last node in `left` and `rtail` pointing to the last node in `right` (both initially `None`).

  2. **Iterate:**
      - Loop through the original linked list starting from the head (`head`).
      - For each node:
          - If the current node's value (`head.val`) is less than `x`:
              - Append the node to the `left` list:
                  - Set `ltail.next` to `head`.
                  - Update `ltail` to point to the newly appended node (`head`).
          - Otherwise (if the current node's value is greater than or equal to `x`):
              - Append the node to the `right` list:
                  - Set `rtail.next` to `head`.
                  - Update `rtail` to point to the newly appended node (`head`).
      - Move on to the next node in the original list by setting `head` to `head.next`.

  3. **Concatenation and Termination:**
      - Connect the `left` list to the `right` list by setting `ltail.next` to `right.next` (assuming `right` is not empty).
      - Set `rtail.next` to `None` to terminate the final linked list.

  4. **Return:**
      - Return the head of the `left` list, which represents the new partitioned linked list.

## Time and Space Complexity:
  **Space Complexity:** O(1)
  This algorithm operates in-place, meaning it modifies the existing linked list nodes without creating any additional linked list structures. It only uses constant extra space for the pointers `ltail` and `rtail`.

  **Time Complexity:** O(n)
  The algorithm iterates through the entire original linked list once (n nodes) in the worst case. There are no nested loops or unnecessary operations, making it linear time complexity.

## C++ Implementation:

  ```
  #include <iostream>

  struct Node {
    int val;
    Node* next;
    Node(int x) : val(x), next(NULL) {}
  };

  Node* partition(Node* head, int x) {
    Node* left  = new Node(0); // Dummy node for left list
    Node* right = new Node(0); // Dummy node for right list
    Node* ltail = left;
    Node* rtail = right;

    while (head) {
      if (head->val < x) {
        ltail->next = head;
        ltail = ltail->next;
      } else {
        rtail->next = head;
        rtail = rtail->next;
      }
      head = head->next;
    }

    // Connect left and right lists
    ltail->next = right->next;

    // Terminate the right list
    rtail->next = NULL;

    // Return head of the left list (new head of the partitioned list)
    return left->next;
  }

  // Example usage
  int main() {
    Node* head = new Node(1);
    head->next = new Node(4);
    head->next->next = new Node(3);
    head->next->next->next = new Node(2);
    head->next->next->next->next = new Node(5);
    head->next->next->next->next->next = new Node(2);

    int x = 3;

    Node* newHead = partition(head, x);

    // Print the partitioned list
    while (newHead) {
      std::cout << newHead->val << " -> ";
      newHead = newHead->next;
    }
    std::cout << "NULL" << std::endl;

    return 0;
  }
  ```

## Alternative Algorithms:
  This in-place partitioning is generally considered the most efficient for linked lists due to its constant space complexity and single pass through the list.
  However, here are some alternative approaches:

  - **Two-pass algorithm:** This approach creates two new linked lists during the first pass (one for elements less than `x` and another for elements greater than/equal to `x`).
      Then, in a second pass, it combines these lists. This has a space complexity of O(n) due to the additional lists.
  - **Recursive approach:**

