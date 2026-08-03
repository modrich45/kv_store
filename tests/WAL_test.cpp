#include "kv/WAL.h"
#include <filesystem>
#include <gtest/gtest.h>
#include <fstream>
#include <unordered_map>

class WALTest : public ::testing::Test
{
protected:
    const std::string wal_file = "test_wal.txt";
    WAL wal{wal_file};

    void SetUp() override
    {
        std::filesystem::remove(wal_file);
    }

    void TearDown() override
    {
        std::filesystem::remove(wal_file);
    }
};

TEST_F(WALTest, LogSetAndReplay)
{
    wal.logSet("name", "Vishal");
    wal.logSet("age", "30");

    std::unordered_map<std::string, std::string> data;
    wal.replay(data);

    EXPECT_EQ(data.size(), 2);
    EXPECT_EQ(data["name"], "Vishal");
    EXPECT_EQ(data["age"], "30");
}

TEST_F(WALTest, LogRemoveAndReplay)
{
    wal.logSet("name", "Vishal");
    wal.logSet("age", "30");
    wal.logRemove("age");

    std::unordered_map<std::string, std::string> data;
    wal.replay(data);

    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data["name"], "Vishal");
    EXPECT_EQ(data.find("age"), data.end());
}

TEST_F(WALTest, LogClearAndReplay)
{
    wal.logSet("name", "Vishal");
    wal.logSet("age", "30");
    wal.logClear();

    std::unordered_map<std::string, std::string> data;
    wal.replay(data);

    EXPECT_TRUE(data.empty());
}

TEST_F(WALTest, ReplayWithInvalidData)
{
    std::ofstream ofs(wal_file);
    ofs << "INVALID DATA\n";
    ofs.close();

    std::unordered_map<std::string, std::string> data;
    bool result = wal.replay(data);

    EXPECT_FALSE(result);
}

TEST_F(WALTest, ClearWAL)
{
    wal.logSet("name", "Vishal");
    wal.logSet("age", "30");

    EXPECT_TRUE(wal.clear());

    std::unordered_map<std::string, std::string> data;
    wal.replay(data);

    EXPECT_TRUE(data.empty());
}

TEST_F(WALTest, LogSetWithInvalidKeyOrValue)
{
    EXPECT_FALSE(wal.logSet("", "value"));
    EXPECT_FALSE(wal.logSet("key", ""));
    EXPECT_FALSE(wal.logSet("key with space", "value"));
    EXPECT_FALSE(wal.logSet("key", "value\nwith\nnewline"));
    EXPECT_FALSE(wal.logSet("key=with=equal", "value"));
}

TEST_F(WALTest, LogRemoveWithInvalidKey)
{
    EXPECT_FALSE(wal.logRemove(""));
    EXPECT_FALSE(wal.logRemove("key with space"));
    EXPECT_FALSE(wal.logRemove("key\nwith\nnewline"));
}

TEST_F(WALTest, LogSetWithEqualSignInKey)
{
    EXPECT_FALSE(wal.logSet("key=with=equal", "value"));
}

TEST_F(WALTest, LogSetWithNewlineInKeyOrValue)
{
    EXPECT_FALSE(wal.logSet("key\nwith\nnewline", "value"));
}