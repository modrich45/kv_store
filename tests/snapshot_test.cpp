#include <filesystem>
#include <gtest/gtest.h>
#include <kv/snapshot.h>

class SnapshotTest : public ::testing::Test
{
protected:
    const std::string snapshot_file = "test_snapshot_only.txt";
    const std::string empty_snapshot_file = "test_snapshot_empty.txt";

    std::unordered_map<std::string, std::string> sample_data{
        {"name", "Vishal"},
        {"age", "30"},
        {"city", "New York"}
    };

    void SetUp() override
    {
        std::filesystem::remove(snapshot_file);
        std::filesystem::remove(empty_snapshot_file);     
    }

    void TearDown() override
    {
        std::filesystem::remove(snapshot_file);
        std::filesystem::remove(empty_snapshot_file);
    }
};

TEST_F(SnapshotTest, SaveAndLoad)
{

    Snapshot snapshot(snapshot_file);

    // Save the data to a file
    ASSERT_TRUE(snapshot.save(sample_data));

    // Clear the original data
    sample_data.clear();

    // Load the data from the file
    ASSERT_TRUE(snapshot.load(sample_data));

    // Verify that the loaded data matches the original data
    EXPECT_EQ(sample_data.size(), 3);
    EXPECT_EQ(sample_data["name"], "Vishal");
    EXPECT_EQ(sample_data["age"], "30");
    EXPECT_EQ(sample_data["city"], "New York");
}

TEST_F(SnapshotTest, LoadNonExistingFile)
{
    Snapshot snapshot("non_existing_file.txt");

    // Attempt to load data from a non-existing file
    ASSERT_FALSE(snapshot.load(sample_data));

    // Verify that the data remains empty
    EXPECT_TRUE(sample_data.empty());
}

TEST_F(SnapshotTest, SaveToInvalidFile)
{
    // Attempt to save data to an invalid file path
    Snapshot snapshot("/invalid_path/test_snapshot.txt");
    ASSERT_FALSE(snapshot.save(sample_data));
}

TEST_F(SnapshotTest, SaveAndLoadEmptyData)
{

    Snapshot snapshot(empty_snapshot_file);

    sample_data.clear(); // Ensure the data is empty

    // Save the empty data to a file
    ASSERT_TRUE(snapshot.save(sample_data));

    // Load the data from the file
    ASSERT_TRUE(snapshot.load(sample_data));

    // Verify that the loaded data is still empty
    EXPECT_TRUE(sample_data.empty());
}