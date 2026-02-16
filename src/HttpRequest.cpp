#include "HttpRequest.h"
#include <sstream>

HttpRequest::HttpRequest(const std::string& raw_request) {
    parse(raw_request);
}

void HttpRequest::parse(const std::string& raw_request) {
    size_t line_end = raw_request.find("\r\n");
    std::string request_line = raw_request.substr(0, line_end);

    std::istringstream stream(request_line);
    stream >> method >> path >> version;
}
