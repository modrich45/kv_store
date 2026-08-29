#include <iostream>
#include "server/tcp_client.h"
#include "server/router.h"
#include<vector>

int main(){   
    TCPClient tcp_client0("127.0.0.1",8080);
    TCPClient tcp_client1("127.0.0.1",8081);
    TCPClient tcp_client2("127.0.0.1",8082);

    if (!tcp_client0.connect()){
        std::cerr << "Failed to start shard 0\n";
        return 1;
    }

    if (!tcp_client1.connect()){
        std::cerr << "Failed to start shard 1\n";
        return 1;
    }

    if (!tcp_client2.connect()){
        std::cerr << "Failed to start shard 2\n";
        return 1;
    }

    std::vector<TCPClient*> tcp_clients;

    tcp_clients.push_back(&tcp_client0);
    tcp_clients.push_back(&tcp_client1);
    tcp_clients.push_back(&tcp_client2);

    Router router(8070,tcp_clients);

    if (!router.start()) return 1;

    router.run();

    router.stop();

    return 0;
}