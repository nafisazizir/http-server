//
//  http_response.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#include "http_response.hpp"
#include <sstream>


namespace http {

const std::string HTTP_VERSION = "HTTP/1.1";
const std::string CRLF = "\r\n";

HTTPResponse::HTTPResponse() : status_code_(HTTPStatusCode::Ok) {}

HTTPResponse::HTTPResponse(HTTPStatusCode status_code) : status_code_(status_code) {}

HTTPResponse::~HTTPResponse() {}

void HTTPResponse::setStatusCode(HTTPStatusCode status_code) {
    status_code_ = status_code;
}

HTTPStatusCode HTTPResponse::getStatusCode() const {
    return status_code_;
}

std::string HTTPResponse::toString() const {
    std::ostringstream oss;
    
    oss << HTTP_VERSION << ' ';
    oss << static_cast<int>(getStatusCode()) << ' ';
    oss << to_string(getStatusCode()) << CRLF;
    for (const auto& header : getHeaders()) {
        oss << header.first << ": " << header.second << CRLF;
    }
    oss << CRLF;
    oss << getContent();
    
    return oss.str();
}

}
