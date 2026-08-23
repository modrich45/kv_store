#include "kv/command_parser.h"

#include <sstream>

Command CommandParser::parse(const std::string& input)
{
    std::istringstream iss(input);
    std::string operation;
    std::string key;
    std::string value;

    iss >> operation >> key;

    if (operation == "SET")
    {
        iss >> value;
        return {CommandType::SET, key, value};
    }
    else if (operation == "GET")
    {
        return {CommandType::GET, key, ""};
    }
    else if (operation == "REMOVE")
    {
        return {CommandType::REMOVE, key, ""};
    }
    else if (operation == "EXISTS")
    {
        return {CommandType::EXISTS, key, ""};
    }
    else if (operation == "SIZE")
    {
        return {CommandType::SIZE, "", ""};
    }
    else if (operation == "CLEAR")
    {
        return {CommandType::CLEAR, "", ""};
    }
    else
    {
        return {CommandType::INVALID, "", ""};
    }
}