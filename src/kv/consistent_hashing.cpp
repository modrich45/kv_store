#include "kv/consistent_hashing.h"

#include <functional>

void ConsistentHashingRing::addNode(const std::string& node) {
    for (int i = 0; i < VIRTUAL_NODES; ++i) {
        std::string virtualNode = node + "#" + std::to_string(i);

        uint64_t hash = std::hash<std::string>{}(virtualNode);

        ring_[hash] = node;
    }
}

void ConsistentHashingRing::removeNode(const std::string& node) {
    for (auto it = ring_.begin(); it != ring_.end();) {
        if (it->second == node) {
            it = ring_.erase(it);
        } else {
            ++it;
        }
    }
}

std::string ConsistentHashingRing::getNode(const std::string& key) const {
    if (ring_.empty()) {
        return "";
    }

    uint64_t hash = std::hash<std::string>{}(key);

    auto it = ring_.lower_bound(hash);

    if (it == ring_.end()) {
        it = ring_.begin();
    }

    return it->second;
}