#include "server/executor.h"

Executor:: Executor(KVStore &store, ReplicaClient* replica_client): store_(store),replica_client_(replica_client){

}

std::string Executor:: executeCommand(const Command &cmd){
    switch (cmd.type){
        case CommandType::SET:{
            store_.set(cmd.key, cmd.value);

            if (replica_client_ != nullptr){
                if (!replica_client_->set(cmd.key,cmd.value)){
                    return "ERROR Replica failed\n";
                }
            }

            return "Data saved successfully\n";
        }

        case CommandType::GET:{
            auto value = store_.get(cmd.key);

            if (!value.has_value())
                return "NULL\n";

            return value.value() + "\n";
        }

        case CommandType::REMOVE:{
            bool removed = store_.remove(cmd.key);

            if (replica_client_ != nullptr){
                if (!replica_client_->remove(cmd.key)){
                    return "ERROR Replica failed\n";
                }
            }

            return removed? "Data deleted successfully\n": "NOT_FOUND\n";
        }

        case CommandType::EXISTS:{
            return store_.exists(cmd.key)? "TRUE\n": "FALSE\n";
        }

        case CommandType::CLEAR:{
            store_.clear();

            if (replica_client_ != nullptr){
                if (!replica_client_->clear()){
                    return "ERROR Replica failed\n";
                }
            }

            return "Data cleared successfully\n";
        }

        case CommandType::SIZE:{
            return std::to_string(store_.size()) + "\n";
        }
    }

    return "ERROR Unknown command\n";
}