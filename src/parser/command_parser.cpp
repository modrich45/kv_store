#include "parser/command_parser.h"
#include<sstream>

Command CommandParser:: parse(std:: string_view input){
    // We are using streams to prevent writing whitespace or typeconversion logic
    std:: istringstream stream{std:: string(input)}; 

    std:: string command;
    stream >> command;

    if(command.empty()){
        throw std:: invalid_argument("Empty command");
    }

    if (command == "SET"){
        std::string key;
        std::string value;

        stream >> key;

        if (key.empty()){
            throw std::invalid_argument("SET requires a key");
        }

        std::getline(stream, value);

        if (!value.empty() && value[0] == ' ') value.erase(0, 1);


        if (value.empty())
            throw std::invalid_argument("SET requires a value");

        return {
            CommandType::SET,
            key,
            value
        };
    }

    if (command == "GET"){
        std::string key;

        stream >> key;

        if (key.empty())
            throw std::invalid_argument("GET requires a key");
        

        return {
            CommandType::GET,
            key,
            ""
        };
    }

    if (command == "REMOVE"){
        std::string key;

        stream >> key;

        if (key.empty())
            throw std::invalid_argument("REMOVE requires a key");
        
        return {
            CommandType::REMOVE,
            key,
            ""
        };
    }

    if (command == "EXISTS"){
        std::string key;

        stream >> key;

        if (key.empty())
            throw std::invalid_argument("EXISTS requires a key");
        

        return {
            CommandType::EXISTS,
            key,
            ""
        };
    }

    if (command == "CLEAR"){
        return {
            CommandType::CLEAR,
            "",
            ""
        };
    }

    if (command == "SIZE"){
        return {
            CommandType::SIZE,
            "",
            ""
        };
    }

    throw std::invalid_argument("Invalid command hai ji");
}