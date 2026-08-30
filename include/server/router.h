#pragma once

#include<vector>
#include "server/tcp_client.h"
#include "server/consistent_hash.h"

class Router{
    private:
        int serverSocket_;
        int port_;
        std:: vector<TCPClient*> tcp_clients_;
        ConsistentHash hash_ring_;

        void handleClient(int clientSocket);

    public:
        explicit Router(const int port, std::vector<TCPClient*> tcp_clients);

        bool start();
        void run();
        void stop();
}; 