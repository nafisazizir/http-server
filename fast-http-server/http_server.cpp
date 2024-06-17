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
#include <fcntl.h>
#include <sys/event.h>
#include <chrono>

namespace http {

http::HTTPServer::HTTPServer(const std::string& host, std::uint16_t port) : host_(host), port_(port), sock_fd_(0), running_(false), rng_(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())), sleep_times_(10, 100) {
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
    
    if (setSocketNonBlocking(sock_fd_) < 0) {
        throw std::runtime_error("Failed set socket to non-blocking");
    }
    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_);
    server_address.sin_addr.s_addr = inet_addr(host_.c_str());
    
    if (bind(sock_fd_, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Failed bind TCP socket");
    }
    
    if (listen(sock_fd_, K_MAX_EVENTS) < 0) {
        std::ostringstream oss;
        oss << "Failed listen on port " << port_;
        throw std::runtime_error(oss.str());
    }
    
    setupKqueue();
    running_ = true;
    listener_thread_ = std::thread(&HTTPServer::startListen, this);
    for (int i = 0; i < K_THREAD_SIZE; i++) {
        worker_threads_[i] = std::thread(&HTTPServer::processEvents, this, i);
    }
    
    std::cout << "Start listening..." << std::endl;
}

void http::HTTPServer::stopServer() {
    running_ = false;
    listener_thread_.join();
    
    for (int i = 0; i < K_THREAD_SIZE; i++) {
        worker_threads_[i].join();
    }
    
    for (int i = 0; i < K_THREAD_SIZE; i++) {
        close(worker_kqueue_fd_[i]);
    }
    
    close(sock_fd_);
}

int HTTPServer::setSocketNonBlocking(int sock_fd) {
    int flags = fcntl(sock_fd, F_GETFL, 0);
    if (flags < 0 || fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -1;
    }
    return 0;
}

void HTTPServer::setupKqueue() {
    for (int i = 0; i < K_THREAD_SIZE; i++) {
        if ((worker_kqueue_fd_[i] = kqueue()) < 0) {
            throw std::runtime_error("Failed to create kqueue file descriptor for worker");
        }
    }
}

void http::HTTPServer::startListen() {
    int client_fd;
    struct sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);
    int current_worker = 0;
    bool active = true;
    
    while (running_) {
        if (!active) {
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_times_(rng_)));
        }
        
        client_fd = accept(sock_fd_, (struct sockaddr *)&client_address, &client_address_len);
        if (client_fd < 0) {
            active = false;
            continue;
        }
        
        if (setSocketNonBlocking(client_fd) < 0) {
            active = false;
            continue;
        }
        
        active = true;
        ClientData *client_data = new ClientData(client_fd);
        controllKqueueEvent(worker_kqueue_fd_[current_worker], EVFILT_READ, EV_ADD | EV_ENABLE, client_fd, client_data);
        current_worker = (current_worker + 1) % K_THREAD_SIZE;
    }
}

void HTTPServer::controllKqueueEvent(int kqueue_fd, int16_t filter, uint16_t flags, int fd, void* udata) {
    struct kevent event;
    
    if (flags & EV_DELETE) {
        EV_SET(&event, fd, filter, flags, 0, 0, udata);
    } else {
        EV_SET(&event, fd, filter, flags, 0, 0, udata);
    }
    
    if (kevent(kqueue_fd, &event, 1, NULL, 0, NULL) < 0) {
        throw std::runtime_error("Failed to control kqueue event");
    }
}

void HTTPServer::processEvents(int worker_id) {
    struct kevent events[K_MAX_EVENTS];
    bool active = true;

    while (running_) {
        if (!active) {
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_times_(rng_)));
        }

        // Wait for events
        int nfds = kevent(worker_kqueue_fd_[worker_id], nullptr, 0, events, K_MAX_EVENTS, nullptr);
        if (nfds <= 0) {
            active = false;
            continue;
        }

        active = true;
        for (int i = 0; i < nfds; ++i) {
            struct kevent &current_event = events[i];
            ClientData *client_data = reinterpret_cast<ClientData *>(current_event.udata);

            if (current_event.flags & EV_EOF || current_event.flags & EV_ERROR) {
                // Connection closed or error occurred
                controllKqueueEvent(worker_kqueue_fd_[worker_id], EVFILT_READ, EV_DELETE, client_data->fd, client_data);
                close(client_data->fd);
                delete client_data;
            } else if (current_event.filter == EVFILT_READ) {
                // Handle read event
                handleClient(client_data->fd);
            } else {
                // Unexpected event
                controllKqueueEvent(worker_kqueue_fd_[worker_id], EVFILT_READ, EV_DELETE, client_data->fd, client_data);
                close(client_data->fd);
                delete client_data;
            }
        }
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
