#pragma once

#include "parser/command_parser.h"
#include "executor.h"
#include<atomic>

class TCPServer{
    private:
        int serverSocket_;
        int port_; 
        std:: atomic<bool> running_;
        Executor& executor_;

        void handleClient(int clientSocket);

    public:
        explicit TCPServer(const int port,Executor &executor);

        bool start();
        void run();
        void stop();
}; 