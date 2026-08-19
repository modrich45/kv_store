#pragma once

class TCPServer{
    private:
        int serverSocket_;
        int port_;

    public:
        explicit TCPServer(int port);

        bool start();
        void run();
        void stop();
}; 