#include "HttpResponse.h"

std::string HttpResponse::buildResponse(int status_code, const std::string& body) {

    std::string status_line;

    if (status_code == 200) {
        status_line = "HTTP/1.1 200 OK\r\n";
    } else {
        status_line = "HTTP/1.1 404 Not Found\r\n";
    }

    std::string headers = "Content-Type: text/plain\r\n"
                          "Content-Length: " + std::to_string(body.size()) + "\r\n"
                          "\r\n";

    return status_line + headers + body;
}