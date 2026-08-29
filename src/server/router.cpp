#include "server/router.h"
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include<thread>
#include<functional>
#include<parser/command_parser.h>

Router :: Router(const int port, std::vector<TCPClient*> tcp_clients) : serverSocket_(-1),port_(port),tcp_clients_(tcp_clients){
}

bool Router:: start(){
    serverSocket_=socket(AF_INET,SOCK_STREAM,0);

    if(serverSocket_==-1){
        std::cerr<<"Router creation failed\n";
        return false;
    }

    sockaddr_in serverAddress{};

    serverAddress.sin_family=AF_INET;

    serverAddress.sin_addr.s_addr=INADDR_ANY;
    serverAddress.sin_port=htons(port_);
    
    if(bind(serverSocket_,reinterpret_cast<sockaddr*>(&serverAddress),sizeof(serverAddress))==-1){
        std:: cerr<<"Bind failed\n";

        close(serverSocket_);
        serverSocket_=-1;

        return false;
    }

    if(listen(serverSocket_,SOMAXCONN)==-1){
        std:: cerr<<"Listen failed\n";

        close(serverSocket_);
        serverSocket_=-1;

        return false;
    }

    std:: cout<<"Server listening on port "<<port_<<"\n";

    return true;
}

void Router::run(){
    while(true){
        sockaddr_in clientAddress{};
        socklen_t clientAddressSize = sizeof(clientAddress);

        int clientSocket=accept(serverSocket_,reinterpret_cast<sockaddr*>(&clientAddress),&clientAddressSize);
        
        if(clientSocket==-1){
            std::cerr << "Accept failed\n";
            return;
        }
        
        std::thread(&Router::handleClient,this,clientSocket).detach();
    }
}

void Router:: stop(){
    if(serverSocket_!=-1){
        close(serverSocket_);
        serverSocket_=-1;
    }
}

void Router:: handleClient(int clientSocket){
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
        
        while ((newlinePosition = receiveBuffer.find('\n'))!= std::string::npos){
            std::string line =receiveBuffer.substr(0, newlinePosition);
            receiveBuffer.erase(0,newlinePosition + 1);
            
            if (line.empty()) continue;
            
            try{
                Command command =CommandParser::parse(line);
                std::string response;
                
                int N=tcp_clients_.size();

                if(N==0) throw std::runtime_error("No shards available");
                std:: string fullLine=line+"\n";

                // handle command conaining key
                if (command.type == CommandType::SET ||command.type == CommandType::GET ||
                    command.type == CommandType::REMOVE ||command.type == CommandType::EXISTS){
                    int node=getNode(command.key);


                    response=tcp_clients_[node]->sendCommand(fullLine);
                }
                // handle command not containing key
                else if(command.type== CommandType:: SIZE){
                    int cnt=0;

                    // stoi is giving error right now
                    for(int i=0;i<N;i++){
                        cnt+=stoi(tcp_clients_[i]->sendCommand(fullLine));
                    }

                    response="Size is: "+cnt;
                }
                else if(command.type==CommandType::CLEAR){
                    for(int i=0;i<N;i++){
                        tcp_clients_[i]->sendCommand(fullLine);
                    }

                    response="Command executed successfully";
                }

                
                send(clientSocket,response.c_str(),response.size(),0);
            }
            catch (const std::invalid_argument& err){
                std::string response ="ERROR " + std::string(err.what()) + "\n";
                send(clientSocket,response.c_str(),response.size(),0);
            }
        }
    }
}

int Router:: getNode(std:: string &key){
    std:: hash<std::string> string_hasher;
    int N=tcp_clients_.size();

    return string_hasher(key)%N;
}