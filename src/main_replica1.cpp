#include <iostream>
#include "kv/kv_store.h"
#include "server/tcp_server.h"
#include "server/executor.h"

int main(){   
    KVStore store(
        "replica_snapshot0.txt",
        "replica_wal0.txt"
    );

    Executor exeutor(store,NULL);

    TCPServer server(8090, exeutor);

    if (!server.start()) return 1;

    server.run();

    server.stop();

    return 0;
}