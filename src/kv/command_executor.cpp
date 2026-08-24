#include "kv/command_executor.h"

CommandExecutor::CommandExecutor(KVStore& store) : store_(store) {}

std::string CommandExecutor::execute(const Command& command)
{
    switch (command.type)
    {
    case CommandType::SET:
        store_.set(command.key, command.value);
        return "OK";
    case CommandType::GET:
    {
        auto value = store_.get(command.key);
        if (value.has_value())
        {
            return value.value();
        }
        else
        {
            return "Key not found";
        }
    }
    case CommandType::REMOVE:
        if (store_.remove(command.key))
        {
            return "OK";
        }
        else
        {
            return "Key not found";
        }
    case CommandType::EXISTS:
        return store_.exists(command.key) ? "true" : "false";
    case CommandType::SIZE:
        return std::to_string(store_.size());
    case CommandType::CLEAR:
        store_.clear();
        return "OK";
    default:
        return "Invalid command";
    }
}