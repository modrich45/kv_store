#pragma once

#include <string>

class ReplicaClient{
    private:
        std::string host_;
        int port_;
        int socket_;

        bool sendCommand(std::string &command, std::string &expResMessage);

    public:
        ReplicaClient(const std::string& host,const int port);

        bool connect();

        bool set(const std::string& key, const std::string& value);

        bool remove(const std::string& key);

        bool clear();

        void disconnect();
};