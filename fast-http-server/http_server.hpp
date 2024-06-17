//
//  http_server.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#ifndef http_server_hpp
#define http_server_hpp

#include <stdio.h>
#include <string>
#include <thread>
#include <random>
#include <netinet/in.h>

#include "http_request.hpp"
#include "http_response.hpp"
#include "uri.hpp"

namespace http {
struct ClientData {
    ClientData(int client_fd) : fd(client_fd) {}
    ~ClientData() {}
    
    int fd;
    sockaddr_in address;
};


class HTTPServer {
public:
    HTTPServer(const std::string& host, std::uint16_t port);
    ~HTTPServer();
    
    void startListen();
    
    std::string getHost() const;
    std::uint16_t getPort() const;
    
private:
    static constexpr int K_THREAD_SIZE = 5;
    static constexpr int K_MAX_EVENTS = 10000;
    
    std::string host_;
    std::uint16_t port_;
    int sock_fd_;
    bool running_;
    std::thread listener_thread_;
    std::thread worker_threads_[K_THREAD_SIZE];
    int worker_kqueue_fd_[K_THREAD_SIZE];
    
    std::mt19937 rng_;
    std::uniform_int_distribution<int> sleep_times_;
    
    
    void startServer();
    void stopServer();
    int setSocketNonBlocking(int sock_fd);
    void setupKqueue();
    void controllKqueueEvent(int kqueue_fd, int16_t filter, uint16_t flags, int fd, void* udata);
    void processEvents(int worker_id);
    
    void handleClient(int client_fd);
    HTTPResponse handleHTTPRequest(const HTTPRequest& request);
};
}

#endif /* http_server_hpp */
