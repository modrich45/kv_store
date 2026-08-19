#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>
#include <iostream>

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

    // 7. Receive data

        char buffer[1024]{};

        int bytes_received = recv(
            client_fd,
            buffer,
            sizeof(buffer) - 1,
            0);

        if (bytes_received > 0)
        {
            buffer[bytes_received] = '\0';

            std::cout << "Client says: "
                      << buffer
                      << '\n';
        }
        else if (bytes_received == 0)
        {
            std::cout << "Client disconnected\n";
        }
        else
        {
            std::cerr << "Receive failed\n";
        }
    

    // 8. Cleanup
    closesocket(client_fd);
    closesocket(server_fd);

    WSACleanup();

    return 0;
}