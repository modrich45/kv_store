#include <gtest/gtest.h>
#include <filesystem>
#include<thread>
#include "kv/kv_store.h"

class KVStoreTest : public ::testing::Test
{
protected:
    const std::string filename = "test_snapshot.txt";
    const std::string wal_filename= "test_wal.txt";
    std::unique_ptr<KVStore> store;

    void SetUp() override
    {
        std::filesystem::remove(filename);
        std::filesystem::remove(wal_filename);
        store = std::make_unique<KVStore>(filename,wal_filename);
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

// ------------------ Thread tests ------------------------------

TEST_F(KVStoreTest, ConcurrentInsertDifferentKeys)
{
    constexpr int numThreads = 10;
    constexpr int keysPerThread = 100;

    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t)
    {
        threads.emplace_back([&, t]()
        {
            for (int i = 0; i < keysPerThread; ++i)
            {
                store->set(
                    "key" + std::to_string(t * keysPerThread + i),
                    "value");
            }
        });
    }

    for (auto &thread : threads)
        thread.join();

    EXPECT_EQ(store->size(), numThreads * keysPerThread);
}

TEST_F(KVStoreTest, ConcurrentReaders){
    store->set("name", "VishalJi");

    constexpr int numThreads = 20;

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back([&]()
        {
            for (int j = 0; j < 1000; ++j)
            {
                auto value = store->get("name");

                ASSERT_TRUE(value.has_value());

                EXPECT_EQ(value.value(), "VishalJi");
            }
        });
    }

    for (auto &thread : threads)
        thread.join();
}

// This testcase is to ensure that the kv system doesn't crashes
TEST_F(KVStoreTest, StressTest)
{
    constexpr int numThreads = 8;
    constexpr int operations = 5000;

    std::vector<std::thread> threads;

    for (int t = 0; t < numThreads; ++t)
    {
        threads.emplace_back([&, t]()
        {
            for (int i = 0; i < operations; ++i)
            {
                std::string key = "key" + std::to_string(i % 100);

                store->set(key, std::to_string(t));

                store->get(key);

                store->exists(key);

                if (i % 10 == 0)
                {
                    store->remove(key);
                }
            }
        });
    }

    for (auto &thread : threads)
        thread.join();
}