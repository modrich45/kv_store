#include<iostream>
#include "server/replica_client.h"
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

ReplicaClient:: ReplicaClient(const std:: string &host, const int port): host_(host), port_(port){
    
}

bool ReplicaClient:: connect(){
    // create client socket
    socket_ = socket(AF_INET,SOCK_STREAM,0);

    if (socket_ == -1) return false;

    sockaddr_in serverAddress{};

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port_);

    // address in host is converted in binary format and stored in sin_addr of serverAddress
    if (inet_pton(AF_INET,host_.c_str(),&serverAddress.sin_addr) <= 0){
        close(socket_);
        socket_ = -1;

        return false;
    }

    // client socket and server are connected
    if (::connect(socket_, reinterpret_cast<sockaddr*>(&serverAddress),sizeof(serverAddress)) == -1){
        close(socket_);
        socket_ = -1;

        return false;
    }

    return true;
}

bool ReplicaClient::set(const std::string& key,const std::string& value){
    std::string command ="SET " + key + " " + value + "\n";
    std:: string expResMessage="Data saved successfully\n";

    return sendCommand(command,expResMessage);
}

bool ReplicaClient::remove(const std::string& key){
    std::string command ="REMOVE " + key + "\n";
    std:: string expResMessage="Data deleted successfully\n";

    return sendCommand(command,expResMessage);
}

bool ReplicaClient::clear(){
    std::string command = "CLEAR\n";
    std:: string expResMessage="Data cleared successfully\n";

    return sendCommand(command,expResMessage);
}

bool ReplicaClient:: sendCommand(std::string &command, std:: string &expResMessage){
    auto sendAll=[](int socket, const std:: string data) -> bool{
        std::size_t totalSent = 0;

        while (totalSent < data.size()){
            ssize_t sent = send(socket,data.data() + totalSent,data.size() - totalSent,0);

            if(sent <= 0) return false;

            totalSent += sent;
        }

        return true;
    };

    if(!sendAll(socket_,command)) return false;

    char buffer[1024];

    ssize_t received = recv(socket_,buffer,sizeof(buffer) - 1,0);

    if (received <= 0) return false;

    buffer[received] = '\0';

    return std::string(buffer) == expResMessage|| std::string(buffer)=="NOT_FOUND\n";
}

void ReplicaClient::disconnect(){
    if (socket_ != -1){
        close(socket_);
        socket_ = -1;
    }
}