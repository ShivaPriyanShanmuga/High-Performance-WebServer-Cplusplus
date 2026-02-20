cmake_minimum_required(VERSION 3.16)
project(cpp_webserver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(webserver
    src/main.cpp
    src/Server.cpp
    src/HttpResponse.cpp
    src/HttpRequest.cpp
)