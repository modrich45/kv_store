#include<gtest/gtest.h>
#include<kv/snapshot.h>

TEST(SnapshotTest, SaveAndLoad)
{

    std::unordered_map<std::string, std::string> data = {
        {"name", "Vishal"},
        {"age", "30"},
        {"city", "New York"}
    };

    Snapshot snapshot("test_snapshot_only.txt");

    // Save the data to a file
    ASSERT_TRUE(snapshot.save(data));

    // Clear the original data
    data.clear();

    // Load the data from the file
    ASSERT_TRUE(snapshot.load(data));

    // Verify that the loaded data matches the original data
    EXPECT_EQ(data.size(), 3);
    EXPECT_EQ(data["name"], "Vishal");
    EXPECT_EQ(data["age"], "30");
    EXPECT_EQ(data["city"], "New York");
}

TEST(SnapshotTest, LoadNonExistingFile)
{
    std::unordered_map<std::string, std::string> data;

    Snapshot snapshot("non_existing_file.txt");

    // Attempt to load data from a non-existing file
    ASSERT_FALSE(snapshot.load(data));

    // Verify that the data remains empty
    EXPECT_TRUE(data.empty());
}

TEST(SnapshotTest, SaveToInvalidFile)
{
    std::unordered_map<std::string, std::string> data = {
        {"name", "Vishal"},
        {"age", "30"},
        {"city", "New York"}
    };

    // Attempt to save data to an invalid file path
    Snapshot snapshot("/invalid_path/test_snapshot.txt");
    ASSERT_FALSE(snapshot.save(data));
}

TEST(SnapshotTest, SaveOnly){
    
    std::unordered_map<std::string, std::string> data = {
        {"name", "Vishal"},
        {"age", "30"},
        {"city", "New York"}
    };

    Snapshot snapshot("test_snapshot_only.txt");

    // Save the data to a file
    ASSERT_TRUE(snapshot.save(data));
}

TEST(SnapshotTest, LoadOnly){
    
    std::unordered_map<std::string, std::string> data;

    Snapshot snapshot("test_snapshot_only.txt");

    // Load the data from the file
    ASSERT_TRUE(snapshot.load(data));

    // Verify that the loaded data matches the original data
    EXPECT_EQ(data.size(), 3);
    EXPECT_EQ(data["name"], "Vishal");
    EXPECT_EQ(data["age"], "30");
    EXPECT_EQ(data["city"], "New York");
}

TEST(SnapshotTest, SaveAndLoadEmptyData)
{
    std::unordered_map<std::string, std::string> data;

    Snapshot snapshot("test_snapshot_empty.txt");

    // Save the empty data to a file
    ASSERT_TRUE(snapshot.save(data));

    // Load the data from the file
    ASSERT_TRUE(snapshot.load(data));

    // Verify that the loaded data is still empty
    EXPECT_TRUE(data.empty());
}