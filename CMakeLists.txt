cmake_minimum_required(VERSION 3.10)

project(HttpServer VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(src)

add_executable(HttpServer
    src/main.cpp
    src/http_server.cpp
    src/http_enums.cpp
    src/http_message_interface.cpp
    src/http_request
    src/http_response.cpp
    src/uri.cpp
)

target_link_libraries(HttpServer PRIVATE Threads::Threads)