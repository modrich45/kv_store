#pragma once

#include<vector>
#include "server/tcp_client.h"

class Router{
    private:
        int serverSocket_;
        int port_;
        std:: vector<TCPClient*> tcp_clients_;

        void handleClient(int clientSocket);
        int getNode(std:: string &key);

    public:
        explicit Router(const int port, std::vector<TCPClient*> tcp_clients);

        bool start();
        void run();
        void stop();
}; 