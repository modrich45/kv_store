#include "kv/command_executor.h"

CommandExecutor::CommandExecutor(KVStore& store) : store_(store) {}

std::string CommandExecutor::execute(const Command& command)
{
    switch (command.type)
    {
    case CommandType::SET:
        store_.set(command.key, command.value);
        return "OK\n";
    case CommandType::GET:
    {
        auto value = store_.get(command.key);
        if (value.has_value())
        {
            return value.value();
        }
        else
        {
            return "Key not found\n";
        }
    }
    case CommandType::REMOVE:
        if (store_.remove(command.key))
        {
            return "OK\n";
        }
        else
        {
            return "Key not found\n";
        }
    case CommandType::EXISTS:
        return store_.exists(command.key) ? "true\n" : "false\n";
    case CommandType::SIZE:
        return std::to_string(store_.size()) + "\n";
    case CommandType::CLEAR:
        store_.clear();
        return "OK\n";
    default:
        return "Invalid command\n";
    }
}