#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>
#include <string>
#include "kv/command_parser.h"
#include "kv/kv_store.h"
#include "kv/command_executor.h"
#include <thread>
#include <functional>

void handle_client(
    SOCKET client_fd,
    KVStore &store)
{
    std::cout << "Client handler started\n";

    std::string receive_buffer;

    CommandParser parser;
    CommandExecutor executor(store);

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
            receive_buffer.append(
                buffer,
                bytes_received);

            size_t pos;

            while ((pos = receive_buffer.find('\n')) != std::string::npos)
            {
                std::string command =
                    receive_buffer.substr(0, pos);

                receive_buffer.erase(
                    0,
                    pos + 1);

                std::cout
                    << "Received command: "
                    << command
                    << '\n';

                Command cmd =
                    parser.parse(command);

                std::string response =
                    executor.execute(cmd);

                response += '\n';

                int bytes_sent = send(
                    client_fd,
                    response.c_str(),
                    static_cast<int>(response.size()),
                    0);

                if (bytes_sent == SOCKET_ERROR)
                {
                    std::cerr
                        << "Send failed: "
                        << WSAGetLastError()
                        << '\n';

                    closesocket(client_fd);
                    return;
                }
            }
        }
        else if (bytes_received == 0)
        {
            std::cout
                << "Client disconnected\n";

            break;
        }
        else
        {
            std::cerr
                << "Receive failed: "
                << WSAGetLastError()
                << '\n';

            break;
        }
    }

    closesocket(client_fd);

    std::cout << "Client handler stopped\n";
}

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

    KVStore store("snapshot.txt", "wal.txt");

    // 7. Handle clients in separate threads
    while (true)
    {
        SOCKET client_fd = accept(
            server_fd,
            nullptr,
            nullptr);

        if (client_fd == INVALID_SOCKET)
        {
            std::cerr
                << "Accept failed: "
                << WSAGetLastError()
                << '\n';

            continue;
        }

        std::cout
            << "Client connected!\n";

        std::thread client_thread(
            handle_client,
            client_fd,
            std::ref(store));

        client_thread.detach();
    }


    WSACleanup();

    return 0;
}