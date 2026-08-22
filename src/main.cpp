#include <iostream>
#include "kv/kv_store.h"
#include "server/tcp_server.h"
#include "server/executor.h"

int main(){   
    KVStore store(
        "snapshot.txt",
        "wal.txt"
    );

    Executor exeutor(store);

    TCPServer server(8080, exeutor);

    if (!server.start()) return 1;

    server.run();

    server.stop();

    return 0;
}