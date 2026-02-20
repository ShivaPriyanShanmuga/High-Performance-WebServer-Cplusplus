#include "Server.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

Server::Server(int port) : port(port) {}

void Server::setupSocket() {

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);

    std::cout << "Server listening on port " << port << std::endl;
}

void Server::start() {
    setupSocket();

    sockaddr_in address;
    int addrlen = sizeof(address);

    while (true) {
        int client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (client_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        char buffer[4096] = {0};
        int bytes_read = read(client_socket, buffer, sizeof(buffer));

        if (bytes_read > 0) {

            std::string raw_request(buffer, bytes_read);
            HttpRequest request(raw_request);

            std::string body;

            if (request.path == "/") {
                body = "Welcome to the home page!\n";
            } else if (request.path == "/hello") {
                body = "Hello from Route /hello\n";
            } else {
                body = "404 Not Found\n";
            }

            HttpResponse response;
            std::string http_response = response.buildResponse(
                request.path == "/" || request.path == "/hello" ? 200 : 404,
                body
            );

            send(client_socket, http_response.c_str(), http_response.size(), 0);

            close(client_socket);
        }
    }
}