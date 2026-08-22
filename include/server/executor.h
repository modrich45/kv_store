#pragma once

#include "kv/kv_store.h"
#include "parser/command_parser.h"

class Executor{
    private:
        KVStore& store_;

    public:
        explicit Executor(KVStore& store);

        std::string executeCommand(const Command &cmd);
};