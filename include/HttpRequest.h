#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>

class HttpRequest {
public:
    std::string method;
    std::string path;
    std::string version;

    HttpRequest(const std::string& raw_request);
    void parse(const std::string& raw_request);
};

#endif