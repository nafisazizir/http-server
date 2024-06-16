//
//  http_request.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#include "http_request.hpp"
#include <sstream>

namespace http {

const std::string HTTP_VERSION = "HTTP/1.1";
const std::string CRLF = "\r\n";

HTTPRequest::HTTPRequest() : version_(HTTP_VERSION), method_(HTTPMethod::GET) {}

HTTPRequest::~HTTPRequest() {}

void HTTPRequest::setMethod(HTTPMethod method) {
    method_ = method;
}

void HTTPRequest::setUri(const Uri& uri) {
    uri_ = uri;
}

HTTPMethod HTTPRequest::getMethod() const {
    return method_;
}

Uri HTTPRequest::getUri() const {
    return uri_;
}

std::string HTTPRequest::toString() const {
    std::ostringstream oss;
    
    oss << to_string(getMethod()) << ' ';
    oss << getUri().getPath() << ' ';
    oss << HTTP_VERSION << CRLF;
    for (const auto& header : getHeaders()) {
        oss << header.first << ": " << header.second << CRLF;
    }
    oss << CRLF;
    oss << getContent();
    
    return oss.str();
}

HTTPRequest string_to_request(const std::string& request_string) {
    std::string start_line;
    std::string header_lines;
    std::string message_body;
    std::istringstream iss;
    HTTPRequest request;
    std::string line;
    std::string method;
    std::string path;
    std::string version = HTTP_VERSION;
    std::string key;
    std::string value;
    Uri uri;
    size_t lpos = 0;
    size_t rpos = 0;
    
    rpos = request_string.find(CRLF);
    if (rpos == std::string::npos) {
        throw std::invalid_argument("Could not find request start line");
    }
    
    start_line = request_string.substr(lpos, rpos - lpos);
    lpos = rpos + 2;
    rpos = request_string.find(CRLF + CRLF, lpos);
    if (rpos != std::string::npos) {
        header_lines = request_string.substr(lpos, rpos - lpos);
        lpos = rpos + 4;
        rpos = request_string.length();
        if (lpos < rpos) {
            message_body = request_string.substr(lpos, rpos - lpos);
        }
    }
    
    iss.clear();
    iss.str(start_line);
    iss >> method >> path >> version;
    if (!iss.good() && iss.eof()) {
        throw std::logic_error("Invalid start line format");
    }
    
    request.setMethod(string_to_method(method));
    request.setUri(Uri(path));
    
    iss.clear();
    iss.str(header_lines);
    while (std::getline(iss, line)) {
        std::istringstream header_stream(line);
        std::getline(header_stream, key, ':');
        std::getline(header_stream, value);
        
        key.erase(std::remove_if(key.begin(), key.end(), [](char c) { return std::isspace(c); }), key.end());
        value.erase(std::remove_if(value.begin(), value.end(), [](char c) { return std::isspace(c); }), value.end());
        request.setHeader(key, value);
    }
    
    request.setContent(message_body);
    return request;
}

}
