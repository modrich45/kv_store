#include "server/tcp_server.h"
#include<sys/socket.h>
#include <netinet/in.h>
#include<iostream>
#include <unistd.h>
#include<thread>

TCPServer :: TCPServer(int port, Executor &executor) : serverSocket_(-1),port_(port),executor_(executor){
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
    serverAddress.sin_addr.s_addr=INADDR_ANY; // already a binary format
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
    running_=true;

    while(running_){

        sockaddr_in clientAddress{};
        socklen_t clientAddressSize = sizeof(clientAddress);

        // When a client comes OS instructs to fill this clientAddress
        int clientSocket=accept(serverSocket_,reinterpret_cast<sockaddr*>(&clientAddress),&clientAddressSize);
        
        if(clientSocket==-1){
            std::cerr << "Accept failed\n";
            return;
        }
        
        std::thread(&TCPServer::handleClient,this,clientSocket).detach();
    }
}

void TCPServer:: stop(){
    if(serverSocket_!=-1){
        close(serverSocket_);
        serverSocket_=-1;
    }
}

void TCPServer:: handleClient(int clientSocket){
    std::cout<<"Client connected\n";
        
    char buffer[1024];
    std::string receiveBuffer;

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
            
        receiveBuffer.append(buffer, bytesReceived);
        std::size_t newlinePosition;
        
        // This will execute only if \n is found because client can complete a single command in more than 1 requests
        while ((newlinePosition = receiveBuffer.find('\n'))!= std::string::npos){
            std::string line =receiveBuffer.substr(0, newlinePosition);
            receiveBuffer.erase(0,newlinePosition + 1);
            
            if (line.empty()) continue;
            
            try{
                Command command =CommandParser::parse(line);
                
                std::string response =executor_.executeCommand(command);
                
                send(clientSocket,response.c_str(),response.size(),0);
            }
            catch (const std::invalid_argument& err){
                std::string response ="ERROR " + std::string(err.what()) + "\n";
                send(clientSocket,response.c_str(),response.size(),0);
            }
        }
    }
    
    close(clientSocket);
}