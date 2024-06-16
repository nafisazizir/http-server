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

#include "http_request.hpp"
#include "http_response.hpp"
#include "uri.hpp"

namespace http {
class HTTPServer {
public:
    HTTPServer(const std::string& host, std::uint16_t port);
    ~HTTPServer();
    
    void startListen();
    
    std::string getHost() const;
    std::uint16_t getPort() const;
    
private:
    std::string host_;
    std::uint16_t port_;
    int sock_fd_;
    
    void startServer();
    void stopServer();
    void handleClient(int client_fd);
    HTTPResponse handleHTTPRequest(const HTTPRequest& request);
};
}

#endif /* http_server_hpp */
