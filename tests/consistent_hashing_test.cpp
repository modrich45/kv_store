#include "kv/consistent_hashing.h"

#include <gtest/gtest.h>

#include <string>
#include <unordered_map>

TEST(ConsistentHashingTest, DistributesKeysAcrossNodes) {
    ConsistentHashingRing ring;

    ring.addNode("127.0.0.1:8080");
    ring.addNode("127.0.0.1:8081");
    ring.addNode("127.0.0.1:8082");

    std::unordered_map<std::string, int> distribution;

    constexpr int NUM_KEYS = 10000;

    for (int i = 0; i < NUM_KEYS; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string node = ring.getNode(key);

        distribution[node]++;
    }

    EXPECT_EQ(distribution.size(), 3);

    for (const auto& [node, count] : distribution) {
        double percentage =
            static_cast<double>(count) / NUM_KEYS * 100.0;

        EXPECT_GT(percentage, 20.0);
        EXPECT_LT(percentage, 45.0);
    }
}

TEST(ConsistentHashingTest, AddingNodeMovesOnlySubsetOfKeys) {
    ConsistentHashingRing ring;

    ring.addNode("127.0.0.1:8080");
    ring.addNode("127.0.0.1:8081");
    ring.addNode("127.0.0.1:8082");

    constexpr int NUM_KEYS = 10000;

    std::unordered_map<std::string, std::string> before;

    for (int i = 0; i < NUM_KEYS; ++i) {
        std::string key = "key" + std::to_string(i);
        before[key] = ring.getNode(key);
    }

    // Add a new node
    ring.addNode("127.0.0.1:8083");

    int movedKeys = 0;

    for (int i = 0; i < NUM_KEYS; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string newNode = ring.getNode(key);

        if (before[key] != newNode) {
            ++movedKeys;
        }
    }

    double movedPercentage =
        static_cast<double>(movedKeys) / NUM_KEYS * 100.0;

    std::cout << "Moved keys: "
              << movedKeys
              << " ("
              << movedPercentage
              << "%)\n";

    EXPECT_GT(movedPercentage, 15.0);
    EXPECT_LT(movedPercentage, 35.0);
}

TEST(ConsistentHashingTest, RemovingNodeReassignsKeys) {
    ConsistentHashingRing ring;

    ring.addNode("127.0.0.1:8080");
    ring.addNode("127.0.0.1:8081");
    ring.addNode("127.0.0.1:8082");

    constexpr int NUM_KEYS = 10000;

    // Record ownership before removing a node
    std::unordered_map<std::string, std::string> before;

    for (int i = 0; i < NUM_KEYS; ++i) {
        std::string key = "key" + std::to_string(i);
        before[key] = ring.getNode(key);
    }

    // Remove one node
    const std::string removedNode = "127.0.0.1:8081";

    ring.removeNode(removedNode);

    int movedKeys = 0;
    int keysFromRemovedNode = 0;

    for (int i = 0; i < NUM_KEYS; ++i) {
        std::string key = "key" + std::to_string(i);

        std::string oldNode = before[key];
        std::string newNode = ring.getNode(key);

        if (oldNode == removedNode) {
            ++keysFromRemovedNode;

            // Keys previously owned by the removed node
            // must now belong to another node.
            EXPECT_NE(newNode, removedNode);

            if (oldNode != newNode) {
                ++movedKeys;
            }
        } else {
            // Keys belonging to surviving nodes should remain
            // on the same node.
            EXPECT_EQ(oldNode, newNode);
        }
    }

    std::cout << "Keys originally owned by "
              << removedNode
              << ": "
              << keysFromRemovedNode
              << "\n";

    std::cout << "Keys reassigned: "
              << movedKeys
              << "\n";

    EXPECT_EQ(keysFromRemovedNode, movedKeys);
}