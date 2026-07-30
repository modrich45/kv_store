#include <kv/snapshot.h>

Snapshot::Snapshot(const std::string& filename) : filename_(std::move(filename)) {}

bool Snapshot::save(const std::unordered_map<std::string, std::string> &data) const
{
    std::ofstream file(filename_, std::ios::trunc);

    if (file.is_open())
    {
        for (const auto &pair : data)
        {
            file << pair.first
                 << '='
                 << pair.second
                 << '\n';
        }
    }
    else
    {
        return false;
    }

    return true;
}

bool Snapshot::load(std::unordered_map<std::string, std::string> &data) const
{
    data.clear();
    std::ifstream file(filename_);

    if (file.is_open())
    {

        std::string line;
        while (std::getline(file, line))
        {
            auto delimiterPos = line.find("=");
            if (delimiterPos == std::string::npos)
            {

                continue;
            }
            auto key = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            data[key] = value;
        }
    }
    else
    {
        return false;
    }

    return true;
}