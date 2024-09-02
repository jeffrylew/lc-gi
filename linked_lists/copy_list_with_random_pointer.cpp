#include <gtest/gtest.h>

#include <functional>
#include <unordered_map>

struct Node
{
    int   val {};
    Node* next {nullptr};
    Node* random {nullptr};

    Node(int val_in)
        : val {val_in}
    {
    }

    Node(int val_in, Node* next_in, Node* random_in)
        : val {val_in}
        , next {next_in}
        , random {random_in}
    {
    }
};

//! @brief Recursive discussion solution to create a deep copy of list
//! @param[in] head Pointer to head of list where each node has a random pointer
//! @return Pointer to head of copied linked list
static Node* copyRandomListDS1(Node* head)
{
    //! @details https://leetcode.com/problems/copy-list-with-random-pointer
    //!
    //!          Time complexity O(N) where N is the number of nodes in list.
    //!          Space complexity O(N). We have recursion stack and space used
    //!          for visited dictionary. Asymptotically, complexity is O(N).

    //! Map holds old nodes as keys and new nodes as its values
    std::unordered_map<Node*, Node*> visited_nodes {};

    std::function<Node*(Node*)> copyRandomList = [&](Node* curr) -> Node* {
        if (curr == nullptr)
        {
            return nullptr;
        }

        //! If we already processed the current node then return cloned version
        if (visited_nodes.contains(curr))
        {
            return visited_nodes[curr];
        }

        //! Create new node with same value as old (copy the node)
        //! @note Memory allocated with new but interface forces its usage
        Node* node = new Node(curr->val);

        //! Save node in hash map. There might be loops during traversal due to
        //! randomness of random pointers.
        visited_nodes[curr] = node;

        //! Recursively copy remaining linked list starting from next pointer
        //! and then from the random pointer. We have two independent recursive
        //! calls. Update next and random pointers for new node created.
        node->next   = copyRandomList(curr->next);
        node->random = copyRandomList(curr->random);

        return node;
    };

    return copyRandomList(head);

} // static Node* copyRandomListDS1( ...

//! @brief Iterative discussion solution with O(N) space
//! @param[in] head Pointer to head of list where each node has a random pointer
//! @return Pointer to head of copied linked list
static Node* copyRandomListDS2(Node* head)
{
    //! @details https://leetcode.com/problems/copy-list-with-random-pointer
    //!
    //!          Time complexity O(N) where N is the number of nodes in list.
    //!          We make one pass over the original linked list.
    //!          Space complexity O(N). We have a dictionary containing mapping
    //!          from old nodes to new nodes. There are N nodes for O(N) space.

    //! Visited dictionary holds old node as key and new node as value
    std::unordered_map<Node*, Node*> visited_nodes {}; 

    std::function<Node*(Node*)> getClonedNode = [&](Node* old_node) {
        if (old_node == nullptr)
        {
            return nullptr;
        }

        //! Check if cloned node is in visited_nodes map
        if (visited_nodes.contains(old_node))
        {
            //! Return new node from map
            return visited_nodes[old_node];
        }

        //! Otherwise, create a new node, save in map, and return it
        visited_nodes[old_node] = new Node(old_node->val);
        return visited_nodes[old_node];
    };

    if (head == nullptr)
    {
        return nullptr;
    }

    auto* old_node = head;

    //! Create new head node
    auto* new_node          = new Node(old_node->val);
    visited_nodes[old_node] = new_node;

    //! Iterate on linked list until all nodes are cloned
    while (old_node != nullptr)
    {
        //! Get clones of nodes referenced by random and next pointers
        new_node->random = getClonedNode(old_node->random);
        new_node->next   = getClonedNode(old_node->next);

        //! Move one step ahead in linked list
        old_node = old_node->next;
        new_node = new_node->next;
    }

    return visited_nodes[head];

} // static Node* copyRandomListDS2( ...

//! @brief Iterative discussion solution with O(1) space
//! @param[in] head Pointer to head of list where each node has a random pointer
//! @return Pointer to head of copied linked list
static Node* copyRandomListDS3(Node* head)
{
    //! @details https://leetcode.com/problems/copy-list-with-random-pointer
    //!
    //!          Time complexity O(N)
    //!          Space complexity O(1). Note this modifies the original list.

    if (head == nullptr)
    {
        return nullptr;
    }

    //! Create new weaved list of original and copied nodes
    Node* ptr = head;
    while (ptr != nullptr)
    {
        //! Cloned node
        Node* new_node = new Node(ptr->val);

        //! Insert cloned node adjacent to original node. If A->B->C is original
        //! list then list after weaving cloned nodes is A->A'->B->B'->C->C'
        new_node->next = ptr->next;
        ptr->next      = new_node;
        ptr            = new_node->next;
    }

    ptr = head;

    //! Now link random pointers of new nodes created. Iterate newly created
    //! list and assign random pointers of cloned nodes to those analogous to
    //! random nodes of original nodes.
    while (ptr != nullptr)
    {
        ptr->next->random =
            (ptr->random != nullptr) ? ptr->random->next : nullptr;
        ptr = ptr->next->next;
    }

    //! Unweave linked list to get original list and cloned list
    Node* ptr_old_list = head;       // A->B->C
    Node* ptr_new_list = head->next; // A'->B'->C'
    Node* head_new     = head->next;

    while (ptr_old_list != nullptr)
    {
        ptr_old_list->next = ptr_old_list->next->next;
        ptr_new_list->next = (ptr_new_list->next != nullptr)
                                 ? ptr_new_list->next->next
                                 : nullptr;

        ptr_old_list = ptr_old_list->next;
        ptr_new_list = ptr_new_list->next;
    }

    return head_new;

} // static Node* copyRandomListDS3( ...

TEST(CopyRandomListTest, SampleTest2)
{
    Node two {2};
    two.random = &two;

    Node one {1};
    one.next   = &two;
    one.random = &two;

    //! @note These result in memory leaks, no "delete" present
    auto* result_DS1 = copyRandomListDS1(&one);
    auto* result_DS2 = copyRandomListDS2(&one);
    auto* result_DS3 = copyRandomListDS3(&one);

    EXPECT_EQ(result_DS1->val, one.val);
    EXPECT_EQ(result_DS1->next->val, one.next->val);
    EXPECT_EQ(result_DS1->random->val, one.random->val);

    EXPECT_EQ(result_DS2->val, one.val);
    EXPECT_EQ(result_DS2->next->val, one.next->val);
    EXPECT_EQ(result_DS2->random->val, one.random->val);

    EXPECT_EQ(result_DS3->val, one.val);
    EXPECT_EQ(result_DS3->next->val, one.next->val);
    EXPECT_EQ(result_DS3->random->val, one.random->val);
}
