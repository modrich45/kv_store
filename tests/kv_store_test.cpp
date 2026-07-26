#include <gtest/gtest.h>

#include "kv/kv_store.h"

TEST(KVStoreTest, InsertNewKey)
{
    KVStore store;

    store.set("name", "Vishal");

    auto value = store.get("name");

    ASSERT_TRUE(value.has_value());

    EXPECT_EQ(value.value(), "Vishal");
}