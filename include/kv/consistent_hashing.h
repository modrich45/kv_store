#pragma once 
#include<string>
#include<map>
#include <cstdint>

class ConsistentHashingRing{
    private:
    static constexpr int VIRTUAL_NODES = 100;
    std::map<uint64_t, std::string> ring_;

    public:
    void addNode(const std::string& node);
    void removeNode(const std::string& node);
    std::string getNode(const std::string& key) const;
};