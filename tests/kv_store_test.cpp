#include <gtest/gtest.h>

#include "kv/kv_store.h"

TEST(KVStoreTest, InsertNewKey)
{
    KVStore store;

    store.set("name", "Vishal");

    auto value = store.get("name");

    ASSERT_TRUE(value.has_value());

    EXPECT_EQ(value.value(), "Vishal");

    EXPECT_EQ(store.size(),1);
}

TEST(KVStoreTest, UpdateExistingKey)
{
    KVStore store;

    store.set("name", "Vishal");

    store.set("name", "John");

    auto value = store.get("name");

    ASSERT_TRUE(value.has_value());

    EXPECT_EQ(value.value(), "John");

    EXPECT_EQ(store.size(),1);
}

TEST(KVStoreTest, RemoveKey)
{
    KVStore store;

    store.set("name", "Vishal");

    bool removed = store.remove("name");

    EXPECT_TRUE(removed);

    auto value = store.get("name");

    EXPECT_FALSE(value.has_value());

    EXPECT_EQ(store.size(),0);
}

TEST(KVStoreTest, RemoveNonExistingKey)
{
    KVStore store;

    bool removed = store.remove("name");

    EXPECT_FALSE(removed);

    EXPECT_EQ(store.size(),0);
}

TEST(KVStoreTest, KeyExists)
{
    KVStore store;

    store.set("name", "Vishal");

    EXPECT_TRUE(store.exists("name"));

    EXPECT_FALSE(store.exists("age"));
}

TEST(KVStoreTest, ClearStore)
{
    KVStore store;

    store.set("name", "Vishal");
    store.set("age", "30");

    EXPECT_EQ(store.size(),2);

    store.clear();

    EXPECT_EQ(store.size(),0);
}

TEST(KVStoreTest, GetNonExistingKey)
{
    KVStore store;

    auto value = store.get("name");

    EXPECT_FALSE(value.has_value());
}

TEST(KVStoreTest, SizeOfEmptyStore)
{
    KVStore store;

    EXPECT_EQ(store.size(),0);
}

TEST(KVStoreTest, SizeAfterMultipleInsertions)
{
    KVStore store;

    store.set("name", "Vishal");
    store.set("age", "30");
    store.set("city", "New York");

    EXPECT_EQ(store.size(),3);
}

TEST(KVStoreTest, SizeAfterRemovals)
{
    KVStore store;

    store.set("name", "Vishal");
    store.set("age", "30");
    store.set("city", "New York");

    EXPECT_EQ(store.size(),3);

    store.remove("age");

    EXPECT_EQ(store.size(),2);

    store.remove("name");

    EXPECT_EQ(store.size(),1);
}

TEST(KVStoreTest, ClearEmptyStore)
{
    KVStore store;

    EXPECT_EQ(store.size(),0);

    store.clear();

    EXPECT_EQ(store.size(),0);
}

TEST(KVStoreTest, ClearAfterRemovals)
{
    KVStore store;

    store.set("name", "Vishal");
    store.set("age", "30");

    EXPECT_EQ(store.size(),2);

    store.remove("age");

    EXPECT_EQ(store.size(),1);

    store.clear();

    EXPECT_EQ(store.size(),0);
}
TEST(KVStoreTest, SetEmptyKey)
{
    KVStore store;

    store.set("", "value");

    EXPECT_EQ(store.size(),0);
}
TEST(KVStoreTest, SetKeyWithEqualSign)
{
    KVStore store;

    store.set("key=with=equal", "value");

    EXPECT_EQ(store.size(),0);
}
TEST(KVStoreTest, SetKeyWithNewline)
{
    KVStore store;

    store.set("key\nwith\nnewline", "value");

    EXPECT_EQ(store.size(),0);
}
TEST(KVStoreTest, SetEmptyValue)
{
    KVStore store;

    store.set("key", "");

    EXPECT_EQ(store.size(),0);
}
TEST(KVStoreTest, SetValueWithNewline)
{
    KVStore store;

    store.set("key", "value\nwith\nnewline");

    EXPECT_EQ(store.size(),0);
}