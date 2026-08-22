#pragma once

#include <string>
#include <string_view>
#include <vector>

enum class CommandType{
    SET,
    GET,
    REMOVE,
    EXISTS,
    CLEAR,
    SIZE
};

struct Command{
    CommandType type;
    std::string key;
    std::string value;
};

class CommandParser{
public:
    static Command parse(std::string_view input);
};