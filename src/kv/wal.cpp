#include<string>
#include "kv/wal.h"
#include<fstream>
#include<string>
#include<string_view>

WAL:: WAL(const std::string& filename) : filename_(filename) {}

bool WAL:: logSet(const std::string& key,const std::string& value) const{
    std::ofstream file(filename_,std::ios::binary|std::ios::app);

    if(!file.is_open()) return false;

    file<<"SET "<<key<<" "<<value<<'\n';

    return true;
}

bool WAL:: logRemove(const std::string& key) const{
    std::ofstream file(filename_,std::ios::binary|std::ios::app);

    if(!file.is_open()) return false;

    file<<"REMOVE "<<key<<'\n';

    return true;
}

bool WAL:: logClear() const{
    std::ofstream file(filename_,std::ios::binary|std::ios::app);

    if(!file.is_open()) return false;

    file<<"CLEAR"<<'\n';

    return true;
}

bool WAL::recover(std::unordered_map<std::string, std::string>& store) const{
    std::ifstream file(filename_);

    if (!file.is_open())
        return false;

    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty())
            return false;

        if (line == "CLEAR")
        {
            store.clear();
            continue;
        }

        std::string setKey="SET ";
        std::string removeKey="REMOVE ";

        if (line.compare(0, setKey.size(), setKey) == 0)
        {
            auto firstSpace = line.find(' ', 4);

            if (firstSpace == std::string::npos)
                return false;

            std::string key = line.substr(4, firstSpace - 4);
            std::string value = line.substr(firstSpace + 1);

            if (key.empty())
                return false;

            store[key] = value;
        }
        else if (line.compare(0, removeKey.size(), removeKey) == 0)
        {
            std::string key = line.substr(7);

            if (key.empty())
                return false;

            store.erase(key);
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool WAL:: clear() const{
    std::ofstream file(filename_,std::ios::trunc);

    if(file.is_open()) return true;

    return false;
}