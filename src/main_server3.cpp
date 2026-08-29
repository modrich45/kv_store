#include <iostream>
#include "kv/kv_store.h"
#include "server/tcp_server.h"
#include "server/executor.h"

int main(){   
    KVStore store(
        "snapshot3.txt",
        "wal3.txt"
    );

    ReplicaClient replica_client("127.0.0.1",8092);

    if (!replica_client.connect()){
        std::cerr << "Failed to connect to replica\n";
        return 1;
    }

    Executor exeutor(store,&replica_client);
    TCPServer server(8082, exeutor);

    if (!server.start()) return 1;

    server.run();

    server.stop();

    return 0;
}