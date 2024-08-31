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

    std::function<Node*(Node*)> copyRandomList = [&](Node* curr) {
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

TEST(CopyRandomListTest, SampleTest2)
{
    Node two {2};
    two.random = &two;

    Node one {1};
    one.next   = &two;
    one.random = &two;

    auto* result_DS1 = copyRandomListDS1(&one);

    EXPECT_EQ(result_DS1->val, one.val);
    EXPECT_EQ(result_DS1->next->val, one.next->val);
    EXPECT_EQ(result_DS1->random->val, one.random->val);
}
