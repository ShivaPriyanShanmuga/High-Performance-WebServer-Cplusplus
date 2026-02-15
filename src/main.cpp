#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int server_fd;
    int port = 8080;

    //  create a socket 
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return 1;
    }

    std::cout << "Socket Created Successfully" << std::endl;

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return 1;
    }

    std::cout << "Bind successful" << std::endl;

    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen Failed" << std::endl;
        return 1;
    }

    std::cout << "Server listening for connections on port " << port << " ..." << std::endl;

    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

    if (new_socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        return 1;
    }

    std::cout << "Client connected" << std::endl;

    std::string body = "Hello from C++ server!\n";

    std::string response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "\r\n" +
        body;

    send(new_socket, response.c_str(), response.size(), 0);


    std::cout << "Message sent to client" << std::endl;

    close(new_socket);
    close(server_fd);

    return 0;
}
