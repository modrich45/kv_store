#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>
#include <string>
#include "kv/command_parser.h"
#include "kv/kv_store.h"
#include "kv/command_executor.h"

int main()
{
    // 1. Initialize Winsock
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // 2. Create TCP socket
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // 3. Server address
    sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // 4. Bind
    if (bind(
            server_fd,
            reinterpret_cast<sockaddr *>(&server_address),
            sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cerr << "Bind failed\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // 5. Listen
    if (listen(server_fd, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "Listen failed\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 8080...\n";

    // 6. Accept client
    SOCKET client_fd = accept(
        server_fd,
        nullptr,
        nullptr);

    if (client_fd == INVALID_SOCKET)
    {
        std::cerr << "Accept failed\n";
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connected!\n";

    std::string receive_buffer;
    CommandParser parser;
    KVStore store("snapshot.txt", "wal.txt");
    CommandExecutor executor(store);
    // 7. Receive data
    while (true)
    {
        char buffer[1024]{};

        int bytes_received = recv(
            client_fd,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytes_received > 0)
        {
            receive_buffer.append(buffer, bytes_received);

            std::string command;
            size_t pos;
            while ((pos = receive_buffer.find('\n')) != std::string::npos)
            {

                command = receive_buffer.substr(0, pos);
                receive_buffer.erase(0, pos + 1);

                std::cout << "Received command: " << command << "\n";

                Command cmd = parser.parse(command);

                std::cout << "Parsed command: Type = " << static_cast<int>(cmd.type)
                          << ", Key = " << cmd.key
                          << ", Value = " << cmd.value << "\n";
                
                std::cout << "Response: " << executor.execute(cmd) << std::endl;

            }
        }
        else if (bytes_received == 0)
        {
            std::cout << "Client disconnected\n";
            break;
        }
        else
        {
            std::cerr << "Receive failed\n";
            break;
        }
    }

    // 8. Cleanup
    closesocket(client_fd);
    closesocket(server_fd);

    WSACleanup();

    return 0;
}