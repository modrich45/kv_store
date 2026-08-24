#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <cstring>
#include <string>


std::string receive_response(
    SOCKET client_socket,
    std::string &receive_buffer)
{
    while (true)
    {
        size_t pos = receive_buffer.find('\n');

        if (pos != std::string::npos)
        {
            std::string response =
                receive_buffer.substr(0, pos);

            receive_buffer.erase(0, pos + 1);

            return response;
        }

        char buffer[1024];

        int bytes_received = recv(
            client_socket,
            buffer,
            sizeof(buffer),
            0);

        if (bytes_received > 0)
        {
            receive_buffer.append(
                buffer,
                bytes_received);
        }
        else if (bytes_received == 0)
        {
            return "SERVER_DISCONNECTED";
        }
        else
        {
            std::cerr << "Receive failed: "
                      << WSAGetLastError()
                      << '\n';

            return "RECEIVE_ERROR";
        }
    }
}

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
    std::string command;
    std::string receive_buffer;

    while (true)
    {
        std::cout << "> ";

        if (!std::getline(std::cin, command))
        {
            break;
        }

        if (command == "exit")
        {
            break;
        }

        command += '\n';

        int bytes_sent = send(
            client_socket,
            command.c_str(),
            static_cast<int>(command.size()),
            0);

        if (bytes_sent == SOCKET_ERROR)
        {
            std::cerr << "Send failed: "
                      << WSAGetLastError()
                      << '\n';
            break;
        }

        std::string response =
            receive_response(
                client_socket,
                receive_buffer);

        if (response == "SERVER_DISCONNECTED")
        {
            std::cout << "Server disconnected\n";
            break;
        }

        if (response == "RECEIVE_ERROR")
        {
            break;
        }

        std::cout << "Server: "
                  << response
                  << '\n';
    }
    // Cleanup
    closesocket(client_socket);
    WSACleanup();

    return 0;
}