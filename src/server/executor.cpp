#include "server/executor.h"

Executor:: Executor(KVStore &store): store_(store){

}

std::string Executor:: executeCommand(const Command &cmd){
    switch (cmd.type){
        case CommandType::SET:{
            store_.set(cmd.key, cmd.value);

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

            return removed? "Data deleted successfully\n": "NOT_FOUND\n";
        }

        case CommandType::EXISTS:{
            return store_.exists(cmd.key)? "TRUE\n": "FALSE\n";
        }

        case CommandType::CLEAR:{
            store_.clear();

            return "Data cleared successfully\n";
        }

        case CommandType::SIZE:{
            return "Number of kv pairs are :"+std::to_string(store_.size()) + "\n";
        }
    }

    return "ERROR Unknown command\n";
}