#pragma once

#include <map>
#include <string>

class ConsistentHash{
private:
    std::map<std::size_t, int> ring_;
    static constexpr int VIRTUAL_NODES = 100;

public:
    void addNode(int node);

    void removeNode(int node);

    int getNode(std::string& key);
};