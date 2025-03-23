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
