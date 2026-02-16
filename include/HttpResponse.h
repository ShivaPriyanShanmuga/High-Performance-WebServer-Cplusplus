#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>

class HttpResponse {
public:
    std::string buildResponse(int status_code, const std::string& body);
};

#endif