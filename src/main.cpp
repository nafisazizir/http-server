//
//  main.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#include <iostream>
#include "http_server.hpp"

int main(int argc, const char *argv[])
{
    std::string host = "127.0.0.1";
    std::uint16_t port = 8080;
    http::HTTPServer server(host, port);
    server.startListen();

    return 0;
}
