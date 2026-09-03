#pragma once

#include <winsock2.h>
#include <string>
#include <vector>

class ReplicationManager
{
private:
    std::vector<SOCKET> replica_sockets;

public:
    ~ReplicationManager();

    bool add_replica(const std::string &ip,int port);

    void replicate(
        const std::string &command);
};