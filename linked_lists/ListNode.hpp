//! @struct ListNode
//! @brief Definition for singly-linked list
struct ListNode
{
    int       val {};
    ListNode* next {nullptr};

    ListNode() = default;

    ListNode(int x)
        : val {x}
    {
    }

    ListNode(int x, ListNode* next_node)
        : val {x}
        , next {next_node}
    {
    }
};
