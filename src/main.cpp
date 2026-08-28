#include <iostream>
#include "kv/kv_store.h"
#include "server/tcp_server.h"
#include "server/executor.h"

int main(){   
    KVStore store(
        "snapshot.txt",
        "wal.txt"
    );

    ReplicaClient replica_client("127.0.0.1",8081);

    if (!replica_client.connect()){
        std::cerr << "Failed to connect to replica\n";
        return 1;
    }

    Executor exeutor(store,&replica_client);

    TCPServer server(8080, exeutor);

    if (!server.start()) return 1;

    server.run();

    server.stop();

    return 0;
}