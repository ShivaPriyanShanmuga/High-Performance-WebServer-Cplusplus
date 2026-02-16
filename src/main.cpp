#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>

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

    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);

        if (new_socket < 0) {
            std::cerr << "Accept failed" << std::endl;
            continue;
        }

        std::cout << "Client Connected" << std::endl;

        char buffer[4096] = {0};

        int bytes_read = read(new_socket, buffer, sizeof(buffer));

        if (bytes_read > 0) {

            std::string request(buffer, bytes_read);

            size_t line_end = request.find("\r\n"); // i know this is not going to be std::string::npos because the first line in a http request is gonna be present
            std::string request_line = request.substr(0, line_end);

            std::cout << "Request Line: " << request_line << std::endl;

            std::istringstream stream(request_line);
            std::string method, path, version;
            stream >> method >> path >> version;

            std::cout << "Method: " << method << std::endl;
            std::cout << "Path: " << path << std::endl;
            std::cout << "Version: " << version << std::endl;

            std::string body;
            if (path == "/") {
                body = "Welcome to the home page!\n";
            } else if (path == "/hello") {
                body = "Hello from Route /hello!\n";
            } else {
                body = "404 Not Found\n";
            }

            std::string response;

            if (path == "/" || path == "/hello") {
                response = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: " + std::to_string(body.size()) + "\r\n"
                    "\r\n" + body;
            } else {
                response = 
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n"
                    "Content-Length: " + std::to_string(body.size()) + "\r\n"
                    "\r\n" + body;
            }

            send(new_socket, response.c_str(), response.size(), 0);
            std::cout << "Response sent" << std::endl;
        }

        close(new_socket);
        std::cout << "Connection Closed!" << std::endl;
    }
}
