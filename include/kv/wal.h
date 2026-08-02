#pragma once

#include <string>
#include <unordered_map>

class WAL
{
public:
    explicit WAL(const std::string& filename);

    bool logSet(const std::string& key,const std::string& value) const;

    bool logRemove(const std::string& key) const;

    bool logClear() const;

    bool recover(std::unordered_map<std::string, std::string>& store) const;

    bool clear() const;

private:
    std::string filename_;
};