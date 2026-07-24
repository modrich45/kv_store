#include <gtest/gtest.h>
#include "../include/kv/kv_store.h"

TEST(KVStoreTest, NewStoreIsEmpty)
{
    KVStore store;

    EXPECT_EQ(store.size(), 0);
    EXPECT_FALSE(store.exists("name"));
}

TEST(KVStoreTest, SetStoresValue)
{
    KVStore store;

    store.set("name", "VishalBoy");

    EXPECT_TRUE(store.exists("name"));
    EXPECT_EQ(store.size(), 1);
}

TEST(KVStoreTest, GetReturnsStoredValue)
{
    KVStore store;

    store.set("name", "VishalBoy");

    auto value = store.get("name");

    ASSERT_TRUE(value.has_value());

    EXPECT_EQ(value.value(), "VishalBoy");
}

TEST(KVStoreTest, GetMissingKey)
{
    KVStore store;

    auto value = store.get("unknown");

    EXPECT_FALSE(value.has_value());
}

TEST(KVStoreTest, RemoveExistingKey)
{
    KVStore store;

    store.set("name", "Lahsiv");

    EXPECT_TRUE(store.remove("name"));

    EXPECT_FALSE(store.exists("name"));

    EXPECT_EQ(store.size(), 0);
}

TEST(KVStoreTest, RemoveMissingKey)
{
    KVStore store;

    EXPECT_FALSE(store.remove("xyz"));
}

TEST(KVStoreTest, ClearRemovesEverything)
{
    KVStore store;

    store.set("a", "1");
    store.set("b", "2");

    store.clear();

    EXPECT_EQ(store.size(), 0);

    EXPECT_FALSE(store.exists("a"));
    EXPECT_FALSE(store.exists("b"));
}