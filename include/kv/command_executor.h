#pragma once

#include <string>
#include "command_parser.h"
#include "kv_store.h"

class CommandExecutor
{
    private:
        KVStore& store_;
    public:
        explicit CommandExecutor(KVStore& store);
        std::string execute(const Command& command);
};