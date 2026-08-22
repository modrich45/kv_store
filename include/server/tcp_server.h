#pragma once

#include "parser/command_parser.h"
#include "executor.h"

class TCPServer{
    private:
        int serverSocket_;
        int port_; 
        
        Executor& executor_;

    public:
        explicit TCPServer(const int port,Executor &executor);

        bool start();
        void run();
        void stop();
}; 