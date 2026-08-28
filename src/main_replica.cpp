#include <iostream>
#include "kv/kv_store.h"
#include "server/tcp_server.h"
#include "server/executor.h"

int main(){   
    KVStore store(
        "replica_snapshot.txt",
        "replica_wal.txt"
    );

    Executor exeutor(store,NULL);

    TCPServer server(8081, exeutor);

    if (!server.start()) return 1;

    server.run();

    server.stop();

    return 0;
}