#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <cstring>

int main()
{
    // Initialize Winsock
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (client_socket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    // Server address
    sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);

    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server_address.sin_addr);

    // Connect to server
    if (connect(
            client_socket,
            reinterpret_cast<sockaddr *>(&server_address),
            sizeof(server_address)) == SOCKET_ERROR)
    {
        std::cerr << "Connection failed\n";

        closesocket(client_socket);
        WSACleanup();

        return 1;
    }

    std::cout << "Connected to server!\n";

    // Send message
    const char *message1 = "SET name Vishal\n";

    const char *message2 = "GET name\n";

    const char *message3 = "SET city Mumbai\n";

    send(
        client_socket,
        message1,
        static_cast<int>(std::strlen(message1)),
        0);

        char buffer[1024];

    int bytes_received = recv(
        client_socket,
        buffer,
        sizeof(buffer) - 1,
        0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        std::cout << "Server: "
                  << buffer
                  << '\n';
    }
    else if (bytes_received == 0)
    {
        std::cout << "Server disconnected\n";
    }
    else
    {
        std::cerr << "Receive failed\n";
    }
    send(
        client_socket,
        message3,
        static_cast<int>(std::strlen(message3)),
        0);

    memset(buffer, 0, sizeof(buffer));
    bytes_received = recv(
        client_socket,
        buffer,
        sizeof(buffer) - 1,
        0);

    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        std::cout << "Server: "
                  << buffer
                  << '\n';
    }
    else if (bytes_received == 0)
    {
        std::cout << "Server disconnected\n";
    }
    else
    {
        std::cerr << "Receive failed\n";
    }

    send(
        client_socket,
        message2,
        static_cast<int>(std::strlen(message2)),
        0);

    memset(buffer, 0, sizeof(buffer));
    bytes_received = recv(
        client_socket,
        buffer,
        sizeof(buffer) - 1,
        0);
    if (bytes_received > 0)
    {
        buffer[bytes_received] = '\0';

        std::cout << "Server: "
                  << buffer
                  << '\n';
    }
    else if (bytes_received == 0)
    {
        std::cout << "Server disconnected\n";
    }
    else
    {
        std::cerr << "Receive failed\n";
    }

    // Cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}