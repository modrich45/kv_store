#include "server/tcp_server.h"
#include<sys/socket.h>
#include <netinet/in.h>
#include<iostream>
#include <unistd.h>

TCPServer :: TCPServer(int port) : serverSocket_(-1),port_(port){
}

bool TCPServer:: start(){
    // Create socket, serverSocket_ wil be assigned a file descripter
    serverSocket_=socket(AF_INET,SOCK_STREAM,0);

    if(serverSocket_==-1){
        std::cerr<<"Socket creation failed\n";
        return false;
    }

    // Create server address
    sockaddr_in serverAddress{};

    serverAddress.sin_family=AF_INET;

    // Accept connections from any network interface
    serverAddress.sin_addr.s_addr=INADDR_ANY;
    serverAddress.sin_port=htons(port_); // We converted port to
    
    // Wrap socket to address
    if(bind(serverSocket_,reinterpret_cast<sockaddr*>(&serverAddress),sizeof(serverAddress))==-1){
        std:: cerr<<"Bind failed\n";

        close(serverSocket_);
        serverSocket_=-1;

        return false;
    }

    // Listening
    if(listen(serverSocket_,SOMAXCONN)){
        std:: cerr<<"Listen failed\n";

        close(serverSocket_);
        serverSocket_=-1;

        return false;
    }

    std:: cout<<"Server listening on port "<<port_<<"\n";

    return true;
}

void TCPServer::run(){
    sockaddr_in clientAddress{};
    socklen_t clientAddressSize = sizeof(clientAddress);

    // When a client comes OS instructs to fill this clientAddress
    int clientSocket=accept(serverSocket_,reinterpret_cast<sockaddr*>(&clientAddress),&clientAddressSize);

    if(clientSocket==-1){
        std::cerr << "Accept failed\n";
        return;
    }

    std::cout<<"Client connected\n";

    char buffer[1024];

    while(true){
        ssize_t bytesReceived = recv(clientSocket,buffer,sizeof(buffer) - 1,0);

        if(bytesReceived==-1){
            std::cerr<<"Receive failed\n";
            close(clientSocket);
            break;
        }

        if(bytesReceived==0){
            std:: cout<<"Client disconnected\n";
            break;
        }

        // To make it a C-type string
        buffer[bytesReceived]='\0';

        std::cout<<"Client sent: "<<buffer<<"\n";

        std::string res="Message was recieved in good hands\n";

        send(clientSocket,res.c_str(),res.size(),0);
    }

    close(clientSocket);
}

void TCPServer:: stop(){
    if(serverSocket_!=-1){
        close(serverSocket_);
        serverSocket_=-1;
    }
}