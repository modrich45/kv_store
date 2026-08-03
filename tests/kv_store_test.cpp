#include <gtest/gtest.h>
#include <filesystem>
#include "kv/kv_store.h"

class KVStoreTest : public ::testing::Test
{
protected:
    const std::string filename = "test_snapshot.txt";
    const std::string wal_filename = "test_wal.txt";
    std::unique_ptr<KVStore> store;

    void SetUp() override
    {
        std::filesystem::remove(filename);
        std::filesystem::remove(wal_filename);
        store = std::make_unique<KVStore>(filename, wal_filename);
    }

    void TearDown() override
    {
        store.reset();
        std::filesystem::remove(filename);
        std::filesystem::remove(wal_filename);
    }
};

TEST_F(KVStoreTest, InsertNewKey)
{
    store->set("name", "Vishal");

    auto value = store->get("name");

    ASSERT_TRUE(value.has_value());

    EXPECT_EQ(value.value(), "Vishal");
    EXPECT_EQ(store->size(), 1);
}

TEST_F(KVStoreTest, UpdateExistingKey)
{
    store->set("name", "Vishal");

    store->set("name", "Vishal");

    store->set("name", "John");

    auto value = store->get("name");

    ASSERT_TRUE(value.has_value());

    EXPECT_EQ(value.value(), "John");

    EXPECT_EQ(store->size(), 1);
}

TEST_F(KVStoreTest, RemoveKey)
{
    store->set("name", "Vishal");

    bool removed = store->remove("name");

    EXPECT_TRUE(removed);

    auto value = store-> get("name");

    EXPECT_FALSE(value.has_value());

    EXPECT_EQ(store->size(), 0);
}

TEST_F(KVStoreTest, RemoveNonExistingKey)
{
    bool removed = store->remove("name");

    EXPECT_FALSE(removed);

    EXPECT_EQ(store->size(), 0);
}

TEST_F(KVStoreTest, KeyExists)
{
    store->set("name", "Vishal");

    EXPECT_TRUE(store->exists("name"));

    EXPECT_FALSE(store->exists("age"));
}

TEST_F(KVStoreTest, ClearStore)
{
    store->set("name", "Vishal");
    store->set("age", "30");

    EXPECT_EQ(store->size(), 2);

    store->clear();

    EXPECT_EQ(store->size(), 0);
}

TEST_F(KVStoreTest, GetNonExistingKey)
{
    auto value = store->get("name");

    EXPECT_FALSE(value.has_value());
}

TEST_F(KVStoreTest, SizeOfEmptyStore)
{
    EXPECT_EQ(store->size(), 0);
}

TEST_F(KVStoreTest, SizeAfterMultipleInsertions)
{

    store->set("name", "Vishal");
    store->set("age", "30");
    store->set("city", "New York");

    EXPECT_EQ(store->size(), 3);
}

TEST_F(KVStoreTest, SizeAfterRemovals)
{
    store->set("name", "Vishal");
    store->set("age", "30");
    store->set("city", "New York");

    EXPECT_EQ(store->size(), 3);

    store->remove("age");

    EXPECT_EQ(store->size(), 2);

    store->remove("name");

    EXPECT_EQ(store->size(), 1);
}

TEST_F(KVStoreTest, ClearEmptyStore)
{
    EXPECT_EQ(store->size(), 0);

    store->clear();

    EXPECT_EQ(store->size(), 0);
}

TEST_F(KVStoreTest, ClearAfterRemovals)
{
    store->set("name", "Vishal");
    store->set("age", "30");

    EXPECT_EQ(store->size(), 2);

    store->remove("age");

    EXPECT_EQ(store->size(), 1);

    store->clear();

    EXPECT_EQ(store->size(), 0);
}
TEST_F(KVStoreTest, SetEmptyKey)
{

    store->set("", "value");

    EXPECT_EQ(store->size(), 0);
}
TEST_F(KVStoreTest, SetKeyWithEqualSign)
{
    store->set("key=with=equal", "value");

    EXPECT_EQ(store->size(), 0);
}
TEST_F(KVStoreTest, SetKeyWithNewline)
{
    store->set("key\nwith\nnewline", "value");

    EXPECT_EQ(store->size(), 0);
}
TEST_F(KVStoreTest, SetEmptyValue)
{
    store->set("key", "");

    EXPECT_EQ(store->size(), 0);
}
TEST_F(KVStoreTest, SetValueWithNewline)
{
    store->set("key", "value\nwith\nnewline");

    EXPECT_EQ(store->size(), 0);
}
TEST_F(KVStoreTest, SetKeyWithSpace)
{
    store->set("key with space", "value");
    EXPECT_EQ(store->size(), 0);
}