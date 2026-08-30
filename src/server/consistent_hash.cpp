#include "server/consistent_hash.h"

void ConsistentHash::addNode(int node){
    for (int i = 0; i < VIRTUAL_NODES; ++i){
        std::string virtual_node =std::to_string(node) + ":" +std::to_string(i);

        std::size_t hash =std::hash<std::string>{}(virtual_node);

        ring_[hash] = node;
    }
}

void ConsistentHash::removeNode(int node){
    for (int i = 0; i < VIRTUAL_NODES; ++i){
        std::string virtual_node =std::to_string(node) + ":" +std::to_string(i);

        std::size_t hash =std::hash<std::string>{}(virtual_node);

        ring_.erase(hash);
    }
}

int ConsistentHash::getNode(std::string& key){
    if (ring_.empty()) throw std::runtime_error("No nodes available");

    std::size_t hash =std::hash<std::string>{}(key);

    auto it = ring_.lower_bound(hash);

    if (it == ring_.end()) it = ring_.begin();

    return it->second;
}