#pragma once

#include "kv/kv_store.h"
#include "parser/command_parser.h"
#include "server/replica_client.h"

class Executor{
    private:
        KVStore& store_;
        ReplicaClient* replica_client_;

    public:
        explicit Executor(KVStore& store, ReplicaClient* repilca_client);

        std::string executeCommand(const Command &cmd);
};