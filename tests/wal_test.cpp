#include <gtest/gtest.h>
#include <filesystem>
#include <unordered_map>

#include "kv/wal.h"

class WALTest : public ::testing::Test
{
protected:
    const std::string filename = "test_wal.log";

    std::unique_ptr<WAL> wal;

    std::unordered_map<std::string, std::string> store;

    void SetUp() override
    {
        std::filesystem::remove(filename);

        wal = std::make_unique<WAL>(filename);
    }

    void TearDown() override
    {
        wal.reset();

        std::filesystem::remove(filename);
    }
};

TEST_F(WALTest, LogSingleSet)
{
    EXPECT_TRUE(wal->logSet("name", "Saksham"));

    EXPECT_TRUE(wal->recover(store));

    ASSERT_EQ(store.size(), 1);

    EXPECT_EQ(store["name"], "Saksham");
}

TEST_F(WALTest, RecoverMultipleSets)
{
    wal->logSet("name", "VishalBoy");
    wal->logSet("city", "Patna");
    wal->logSet("age", "23");

    EXPECT_TRUE(wal->recover(store));

    EXPECT_EQ(store.size(), 3);

    EXPECT_EQ(store["name"], "VishalBoy");
    EXPECT_EQ(store["city"], "Patna");
    EXPECT_EQ(store["age"], "23");
}

TEST_F(WALTest, RecoverUpdatedKey)
{
    wal->logSet("name", "Saksham");
    wal->logSet("name", "Vishal");

    EXPECT_TRUE(wal->recover(store));

    ASSERT_EQ(store.size(), 1);

    EXPECT_EQ(store["name"], "Vishal");
}

TEST_F(WALTest, RecoverRemove)
{
    wal->logSet("name", "Saksham");
    wal->logRemove("name");

    EXPECT_TRUE(wal->recover(store));

    EXPECT_TRUE(store.empty());
}

TEST_F(WALTest, RecoverRemoveNonExistingKey)
{
    wal->logRemove("name");

    EXPECT_TRUE(wal->recover(store));

    EXPECT_TRUE(store.empty());
}

TEST_F(WALTest, RecoverClear)
{
    wal->logSet("name", "Saksham");
    wal->logSet("city", "Meerut");

    wal->logClear();

    EXPECT_TRUE(wal->recover(store));

    EXPECT_TRUE(store.empty());
}