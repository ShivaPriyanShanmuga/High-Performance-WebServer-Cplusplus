#ifndef SERVER_H
#define SERVER_H

class Server {
    int port;
    int server_fd;

public:
    Server(int port);
    void start();

private:
    void setupSocket();
};

#endif