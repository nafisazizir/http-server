//
//  http_server.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#include "http_server.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sstream>
#include <unistd.h>
#include <iostream>

namespace http {

http::HTTPServer::HTTPServer(const std::string& host, std::uint16_t port) : host_(host), port_(port), sock_fd_(0) {
    startServer();
}

http::HTTPServer::~HTTPServer() {
    stopServer();
}

void http::HTTPServer::startServer() {
    if ((sock_fd_ = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw std::runtime_error("Failed create TCP socket");
    }
    
    // Set options to reuse socket from previous socket (if applicable)
    // SO_REUSEADDR is specifically for MacOS. It might differ from another OS
    int opt = 1;
    if (setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed set socket options: " + std::string(strerror(errno)));
    }
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_);
    server_address.sin_addr.s_addr = inet_addr(host_.c_str());
    
    if (bind(sock_fd_, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Failed bind TCP socket");
    }
}

void http::HTTPServer::stopServer() {
    close(sock_fd_);
    exit(0);
}

void http::HTTPServer::startListen() {
    if (listen(sock_fd_, SOMAXCONN) < 0) {
        std::ostringstream oss;
        oss << "Failed listen on port " << port_;
        throw std::runtime_error(oss.str());
    }
    
    std::cout << "Start listening..." << std::endl;
    
    int client_fd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    
    while (true) {
        client_fd = accept(sock_fd_, (sockaddr *)&client_address, &client_address_len);
        if (client_fd < 0) {
            continue;
        }
        
        // continue handle client / handle http request
        handleClient(client_fd);
        close(client_fd);
    }
}

void HTTPServer::handleClient(int client_fd) {
    // TODO: properly read request from client
    char buffer[1024] = {0};
    long bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        std::cout << "Error receiving request from client" << std::endl;
        return; // Failed received data from client
    }
    
    // TODO: parse raw request to Request
    // TODO: process Request to Response
    
    HTTPResponse response = HTTPResponse();
    
    // Plain text placeholder
    response.setHeader("Content-Type", "text/plain");
    response.setContent("G'day Mate!");
    
    // HTML placeholder
    // response.setHeader("Content-Type", "text/html");
    // response.setContent("<!DOCTYPE html><html lang=\"en\"><body><h1> G'day Mate!</h1></body></html>");
    
    std::string response_str = response.toString();
    
    long bytes_sent = send(client_fd, response_str.c_str(), response_str.size(), 0);
    if (bytes_sent != response_str.size()) {
        std::cout << "Error sending response to client" << std::endl;
    }
}

// Getter
std::string http::HTTPServer::getHost() const {
    return host_;
}

std::uint16_t http::HTTPServer::getPort() const {
    return port_;
}

}
