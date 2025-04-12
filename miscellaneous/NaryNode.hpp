#include <utility>
#include <vector>

//! @struct NaryNode
//! @brief Node in an n-ary tree
struct NaryNode
{
    int                    val {};
    std::vector<NaryNode*> children;

    NaryNode() = default;

    constexpr explicit NaryNode(int val_in)
        : val {val_in}
    {
    }

    explicit NaryNode(int val_in, std::vector<NaryNode*> children_in)
        : val {val_in}
        , children {std::move(children_in)}
    {
    }
};
