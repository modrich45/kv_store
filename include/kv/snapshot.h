#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>

class Snapshot {
    public:
        explicit Snapshot(const std::string& filename);

        bool save(const std::unordered_map<std::string, std::string>& data) const;

        bool load(std::unordered_map<std::string, std::string>& data) const;

    private:
        const std::string filename_;
};