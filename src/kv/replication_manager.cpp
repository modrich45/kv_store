#include "kv/replication_manager.h"

#include <ws2tcpip.h>
#include <iostream>
ReplicationManager::~ReplicationManager()
{
    for (SOCKET socket_fd : replica_sockets)
    {
        closesocket(socket_fd);
    }
}

bool ReplicationManager::add_replica(
    const std::string &ip,
    int port)
{
    SOCKET socket_fd =
        socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == INVALID_SOCKET)
    {
        std::cerr << "Failed to create replica socket\n";
        return false;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_port = htons(port);

    inet_pton(
        AF_INET,
        ip.c_str(),
        &address.sin_addr);

    if (connect(
            socket_fd,
            reinterpret_cast<sockaddr *>(&address),
            sizeof(address)) == SOCKET_ERROR)
    {
        std::cerr
            << "Failed to connect to replica "
            << ip << ":" << port
            << " Error: "
            << WSAGetLastError()
            << '\n';

        closesocket(socket_fd);

        return false;
    }

    replica_sockets.push_back(socket_fd);

    std::cout
        << "Connected to replica "
        << ip << ":" << port
        << '\n';

    return true;
}

void ReplicationManager::replicate(
    const std::string &command)
{
    std::string replication_command =
        "REPL " + command + "\n";

    for (SOCKET replica : replica_sockets)
    {
        int bytes_sent = send(
            replica,
            replication_command.c_str(),
            static_cast<int>(replication_command.size()),
            0);

        if (bytes_sent == SOCKET_ERROR)
        {
            std::cerr
                << "Replication failed: "
                << WSAGetLastError()
                << '\n';
        }
    }
}