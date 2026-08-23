#pragma once

#include <string>
#include <vector>

enum class CommandType
{
    SET,
    GET,
    REMOVE,
    EXISTS,
    SIZE,
    CLEAR,
    INVALID
};

struct Command{
    CommandType type;
    std::string key;
    std::string value;
};

class CommandParser
{
    public:
        Command parse(const std::string& input);
};