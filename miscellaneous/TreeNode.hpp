struct TreeNode
{
    int       val {};
    TreeNode* left {nullptr};
    TreeNode* right {nullptr};

    TreeNode() = default;

    TreeNode(int x) : val {x} {}

    TreeNode(int x, TreeNode* left_in, TreeNode* right_in)
        : val {x}
        , left {left_in}
        , right {right_in}
    {
    }
};
