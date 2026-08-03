#include<fstream>
#include "kv/WAL.h"

WAL::WAL(const std::string& filename) : filename_(filename) {}

bool WAL::logSet(const std::string& key, const std::string& value) const {
    std::ofstream ofs(filename_, std::ios::app);
    if(!ofs.is_open()) {
        return false;
    }
    if(key.empty() || value.empty() || key.find(' ') != std::string::npos || key.find('\n') != std::string::npos || value.find('\n') != std::string::npos || key.find('=') != std::string::npos) {
        return false;
    }
    ofs << "SET " << key << " " << value << "\n";
    return ofs.good();
}

bool WAL::logRemove(const std::string& key) const {
    std::ofstream ofs(filename_, std::ios::app);
    if(!ofs.is_open()) {
        return false;
    }
    if(key.empty() || key.find(' ') != std::string::npos || key.find('\n') != std::string::npos) {
        return false;
    }
    ofs << "REMOVE " << key << "\n";
    return ofs.good();
}

bool WAL::logClear() const {
    std::ofstream ofs(filename_, std::ios::app);
    if(!ofs.is_open()) {
        return false;
    }
    ofs << "CLEAR\n";
    return ofs.good();
}

bool WAL::replay(std::unordered_map<std::string, std::string>& data) const {
    std::ifstream ifs(filename_);
    if(!ifs.is_open()) {
        return false;
    }
    std::string line;
    while(std::getline(ifs, line)) {
        std::string key, value;
        if(line.compare(0, 4, "SET ") == 0) {
            size_t first_space = line.find(' ', 4);
            if(first_space != std::string::npos) {
                key = line.substr(4, first_space - 4);
                value = line.substr(first_space + 1);
                data[key] = value;
            }
        } else if(line.compare(0, 6, "REMOVE") == 0) {
            key = line.substr(7);
            data.erase(key);
        } else if(line == "CLEAR") {
            data.clear();
        }else{
            return false;
        }
    }
    return true;
}

bool WAL::clear() const {
    std::ofstream ofs(filename_, std::ios::trunc);
    return ofs.is_open();
}