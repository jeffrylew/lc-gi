#include <gtest/gtest.h>

#include <queue>
#include <unordered_map>

//! @class LRUCacheFA
//! @brief First attempt solution does not pass SampleTest3
class LRUCacheFA
{
public:
    LRUCacheFA(int capacity)
        : cache_capacity {capacity}
    {
    }

    int get(int key)
    {
        if (!cache.contains(key))
        {
            return -1;
        }

        most_recent_key = key;
        key_order.push(key);
        return cache[key];
    }

    void put(int key, int value)
    {
        if (!cache.contains(key) && cache_capacity == std::ssize(cache))
        {
            //! Capacity will be exceeded so need to evict oldest key
            while (!key_order.empty()
                   && (key_order.front() == most_recent_key
                       || !cache.contains(key_order.front())))
            {
                key_order.pop();
            }

            if (!key_order.empty())
            {
                cache.erase(key_order.front());
                key_order.pop();
            }
            else
            {
                cache.erase(most_recent_key);
            }
        }

        cache[key]      = value;
        most_recent_key = key;
        key_order.push(key);
    }

private:
    //! Store capacity of LRU cache
    int cache_capacity {};

    //! Keep track of most recently accessed key
    //! @note A stack for most recently accessed keys via get() is likely needed
    int most_recent_key {};

    //! map keeps track of key-value pairs
    std::unordered_map<int, int> cache;

    //! Keep track of order of keys
    std::queue<int> key_order;
};

struct NodeDS1
{
    int      key {};
    int      val {};
    NodeDS1* prev {nullptr};
    NodeDS1* next {nullptr};

    NodeDS1() = default;

    explicit NodeDS1(int key_in, int val_in)
        : key {key_in}
        , val {val_in}
    {
    }
};

//! @class LRUCacheDS1
//! @brief Doubly linked list discussion solution
//! @details Time complexity O(1) for both get and put.
//!          Space complexity O(capacity). We use up to O(capacity) for the hash
//!          map and for the linked list.
class LRUCacheDS1
{
public:
    LRUCache(int capacity)
        : cache_capacity {capacity}
    {
        head.next = &tail;
        tail.prev = &head;
    }

    int get(int key)
    {
        if (!key_node_map.contains(key))
        {
            return -1;
        }

        //! Move node associated with key to back of linked list
        auto& node = key_node_map[key];
        remove(node);
        add(node);

        return node.val;
    }

    void put(int key, int value)
    {
        //! Check if key already exists. If it does, remove its associated node.
        //! We will move the node to the back of the list.
        if (key_node_map.contains(key))
        {
            remove(key_node_map[key]);
        }

        //! Create new node, add it to the hash map, and add it to linked list
        key_node_map[key] = NodeDS1 {key, value};
        add(key_node_map[key]);

        if (std::ssize(key_node_map) > cache_capacity)
        {
            //! If capacity exceeded, delete first real node from linked list
            auto& node_to_delete = *head.next;
            remove(node_to_delete);
            key_node_map.erase(node_to_delete.key);
        }
    }

private:
    //! Store capacity of LRU cache
    int cache_capacity {};

    //! Map of <key, corresponding NodeDS1>
    std::unordered_map<int, NodeDS1> key_node_map;

    //! Sentinel nodes for linked list
    NodeDS1 head;
    NodeDS1 tail;

    //! @brief Add node to end of linked list
    //! @param[in, out] node Reference to NodeDS1 to add
    void add(NodeDS1& node)
    {
        //! Get current (non-sentinel) node at end of linked list
        auto* prev_end = tail.prev;

        //! Insert node after prev_end
        prev_end->next = &node;

        //! Now set pointers of node
        node.prev = prev_end;
        node.next = &tail;

        //! Finally, update tail.prev
        tail.prev = &node;
    }

    //! @brief Remove node from linked list
    //! @param[in, out] node Reference to NodeDS1 to add
    void remove(NodeDS1& node)
    {
        node.prev->next = node.next;
        node.next->prev = node.prev;
    }
};

TEST(LRUCacheTest, SampleTest1)
{
    LRUCacheFA lru_cache_fa(2);
    lru_cache_fa.put(1, 1); // cache is {1: 1}
    lru_cache_fa.put(2, 2); // cache is {1: 1, 2: 2}
    EXPECT_EQ(1, lru_cache_fa.get(1));

    lru_cache_fa.put(3, 3); // LRU key was 2. Evict key 2, cache is {1: 1, 3: 3}
    EXPECT_EQ(-1, lru_cache_fa.get(2)); // returns -1 (not found)

    lru_cache_fa.put(4, 4); // LRU key was 1. Evict key 1, cache is {4: 4, 3: 3}
    EXPECT_EQ(-1, lru_cache_fa.get(1)); // returns -1 (not found)
    EXPECT_EQ(3, lru_cache_fa.get(3));  // returns 3
    EXPECT_EQ(4, lru_cache_fa.get(4));  // returns 4
}

TEST(LRUCacheTest, SampleTest2)
{
    LRUCacheFA lru_cache_fa(1);
    lru_cache_fa.put(2, 1); // cache is {2: 1}
    EXPECT_EQ(1, lru_cache_fa.get(2));

    lru_cache_fa.put(3, 2); // cache is {3: 2}
    EXPECT_EQ(-1, lru_cache_fa.get(2)); // returns -1 (not found)
    EXPECT_EQ(2, lru_cache_fa.get(3));  // returns 2
}

TEST(LRUCacheTest, SampleTestTest3)
{
    LRUCacheFA lru_cache_fa(3);
    lru_cache_fa.put(1, 1); // cache is {1: 1}
    lru_cache_fa.put(2, 2); // cache is {1: 1, 2: 2}
    lru_cache_fa.put(3, 3); // cache is {1: 1, 2: 2, 3: 3}
    lru_cache_fa.put(4, 4); // cache is {4: 4, 2: 2, 3: 3}
    EXPECT_EQ(4, lru_cache_fa.get(4));  // returns 4
    EXPECT_EQ(3, lru_cache_fa.get(3));  // returns 3
    EXPECT_EQ(2, lru_cache_fa.get(2));  // returns 2
    EXPECT_EQ(-1, lru_cache_fa.get(1)); // returns -1 (not found)

    lru_cache_fa.put(5, 5); // cache is {5: 5, 2: 2, 3: 3}
    EXPECT_EQ(-1, lru_cache_fa.get(1));
    EXPECT_EQ(2, lru_cache_fa.get(2));

    EXPECT_NE(3, lru_cache_fa.get(3));
    EXPECT_EQ(-1, lru_cache_fa.get(3));

    EXPECT_NE(-1, lru_cache_fa.get(4));
    EXPECT_EQ(4, lru_cache_fa.get(4));

    EXPECT_EQ(5, lru_cache_fa.get(5));
}
