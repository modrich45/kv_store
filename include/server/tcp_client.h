#pragma once

#include <string>

class TCPClient{
    private:
        std::string host_;
        int port_;
        int socket_;

        
        public:
        TCPClient(const std::string& host,const int port);

        bool connect();
        
        void disconnect();

        std::string sendCommand(std::string &command);
};