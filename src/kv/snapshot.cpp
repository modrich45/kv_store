#include <string>
#include <unordered_map>
#include <kv/snapshot.h>
#include<fstream>

Snapshot::Snapshot(const std::string& filename) : filename_(filename) {}

bool Snapshot::save(const std::unordered_map<std::string, std::string>& data) const{
    std::ofstream file(filename_,std::ios::binary|std::ios::trunc);

    char c='=';

    if(!file.is_open()) return false;

    for (const auto& [key, value] : data){
        file.write(key.data(), key.size());
        file.put('=');
        file.write(value.data(), value.size());
        file.put('\n');
    }

    file.close();

    return true;
}

bool Snapshot::load(std::unordered_map<std::string, std::string>& data) const{
    std::ifstream file(filename_,std::ios::binary);

    data.clear();
    if(!file.is_open()) return false;

    std::string line;

    while(std::getline(file,line)){
        auto delimiterPos=line.find('=');

        if(delimiterPos==std::string::npos) return false;

        data[line.substr(0,delimiterPos)]=line.substr(delimiterPos+1);
    }

    file.close();

    return true;
}