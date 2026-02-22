#include "Server.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

Server::Server(int port) : port(port) {}

void Server::setupSocket() {

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int flags = fcntl(server_fd, F_GETFL, 0);
    fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

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

    // sockaddr_in address;
    // int addrlen = sizeof(address);

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll creation failed" << std::endl;
        return;
    }

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_fd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    const int MAX_EVENTS = 10;
    epoll_event events[MAX_EVENTS];

    while (true) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < event_count; ++i) {

            if (events[i].data.fd == server_fd) {
                int client_socket = accept(server_fd, nullptr, nullptr);
                if (client_socket < 0) {
                    continue;
                }

                int flags = fcntl(client_socket, F_GETFL, 0);
                fcntl(client_socket, F_SETFL, flags | O_NONBLOCK);

                epoll_event client_event;
                client_event.events = EPOLLIN;
                client_event.data.fd = client_socket;

                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_socket, &client_event);

            } else {
                int client_socket = events[i].data.fd;
                char buffer[4096] = {0};
                int bytes_read = read(client_socket, buffer, sizeof(buffer));

                if (bytes_read <= 0) {
                    close(client_socket);
                    continue;
                }

                std::string raw_request(buffer, bytes_read);
                HttpRequest request(raw_request);

                std::string body;

                if (request.path == "/") {
                    body = "Welcome to epoll server!\n";
                } else {
                    body = "Hello from high-performance server!\n";
                }

                HttpResponse response;
                std::string response_text = response.buildResponse(200, body);

                send(client_socket, response_text.c_str(), response_text.size(), 0);
                close(client_socket);
            }

        }
    }
}