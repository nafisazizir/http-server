//
//  http_message.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#include "http_message.hpp"

#include <algorithm>
#include <cctype>
#include <sstream>

namespace http {

const std::string CRLF = "\r\n";
const std::string HTTP_VERSION = "HTTP/1.1";

// Interface
http::HTTPMessageInterface::HTTPMessageInterface() {}
http::HTTPMessageInterface::~HTTPMessageInterface() {}

void http::HTTPMessageInterface::setHeader(const std::string& key, const std::string& value) {
    headers_[key] = std::move(value);
}

void http::HTTPMessageInterface::removeHeader(const std::string& key) {
    headers_.erase(key);
}

void http::HTTPMessageInterface::clearHeader() {
    headers_.clear();
}

void http::HTTPMessageInterface::setContent(const std::string& content) {
    content_ = std::move(content);
    setContentLength();
}

void http::HTTPMessageInterface::clearContent() {
    content_.clear();
}

std::string http::HTTPMessageInterface::getHeader(const std::string& key) const {
    if (headers_.count(key) > 0) {
        return headers_.at(key);
    }
    return std::string();
}

std::map<std::string, std::string> http::HTTPMessageInterface::getHeaders() const {
    return headers_;
}

std::string http::HTTPMessageInterface::getContent() const {
    return content_;
}

size_t http::HTTPMessageInterface::getContentLength() const {
    return content_.length();
}

void http::HTTPMessageInterface::setContentLength() {
    setHeader("Content-Length", std::to_string(content_.length()));
}

// Request
http::HTTPRequest::HTTPRequest() : version_(HTTP_VERSION), method_(HTTPMethod::GET) {}

http::HTTPRequest::~HTTPRequest() {}

void http::HTTPRequest::setMethod(HTTPMethod method) {
    method_ = method;
}

void http::HTTPRequest::setUri(const Uri& uri) {
    uri_ = uri;
}

http::HTTPMethod HTTPRequest::getMethod() const {
    return method_;
}

Uri http::HTTPRequest::getUri() const {
    return uri_;
}

// Response
http::HTTPResponse::HTTPResponse() : status_code_(HTTPStatusCode::Ok) {}

http::HTTPResponse::HTTPResponse(HTTPStatusCode status_code) : status_code_(status_code) {}

http::HTTPResponse::~HTTPResponse() {}

void http::HTTPResponse::setStatusCode(HTTPStatusCode status_code) {
    status_code_ = status_code;
}

HTTPStatusCode http::HTTPResponse::getStatusCode() const {
    return status_code_;
}

std::string http::HTTPResponse::toString() {
    std::ostringstream oss;
    
    oss << "HTTP/1.1" << ' ';
    oss << static_cast<int>(getStatusCode()) << ' ';
    oss << to_string(getStatusCode()) << "\r\n";
    for (const auto& header : getHeaders()) {
        oss << header.first << ": " << header.second << "\r\n";
    }
    oss << "\r\n";
    oss << getContent();
    
    return oss.str();
}

// Implement utility functions
std::string to_string(HTTPMethod method) {
    switch (method) {
        case HTTPMethod::GET:
          return "GET";
        case HTTPMethod::HEAD:
          return "HEAD";
        case HTTPMethod::POST:
          return "POST";
        case HTTPMethod::PUT:
          return "PUT";
        case HTTPMethod::DELETE:
          return "DELETE";
        case HTTPMethod::CONNECT:
          return "CONNECT";
        case HTTPMethod::OPTIONS:
          return "OPTIONS";
        case HTTPMethod::TRACE:
          return "TRACE";
        case HTTPMethod::PATCH:
          return "PATCH";
        default:
          return std::string();
    };
}

std::string to_string(HTTPStatusCode status_code) {
    switch (status_code) {
        case HTTPStatusCode::Continue:
          return "Continue";
        case HTTPStatusCode::Ok:
          return "OK";
        case HTTPStatusCode::Accepted:
          return "Accepted";
        case HTTPStatusCode::MovedPermanently:
          return "Moved Permanently";
        case HTTPStatusCode::Found:
          return "Found";
        case HTTPStatusCode::BadRequest:
          return "Bad Request";
        case HTTPStatusCode::Forbidden:
          return "Forbidden";
        case HTTPStatusCode::NotFound:
          return "Not Found";
        case HTTPStatusCode::MethodNotAllowed:
          return "Method Not Allowed";
        case HTTPStatusCode::ImATeapot:
          return "I'm a Teapot";
        case HTTPStatusCode::InternalServerError:
          return "Internal Server Error";
        case HTTPStatusCode::NotImplemented:
          return "Not Implemented";
        case HTTPStatusCode::BadGateway:
          return "Bad Gateway";
        default:
          return std::string();
    };
}

HTTPMethod string_to_method(const std::string& method_string) {
    std::string method_string_uppercase = method_string;
    std::transform(method_string_uppercase.begin(), method_string_uppercase.end(), method_string_uppercase.begin(), [](char c) {return std::toupper(c); });
    
    if (method_string_uppercase == "GET") {
        return HTTPMethod::GET;
    }
    else if (method_string_uppercase == "HEAD") {
        return HTTPMethod::HEAD;
    }
    else if (method_string_uppercase == "POST") {
        return HTTPMethod::POST;
    }
    else if (method_string_uppercase == "PUT") {
        return HTTPMethod::PUT;
    }
    else if (method_string_uppercase == "DELETE") {
        return HTTPMethod::DELETE;
    }
    else if (method_string_uppercase == "CONNECT") {
        return HTTPMethod::CONNECT;
    }
    else if (method_string_uppercase == "OPTIONS") {
        return HTTPMethod::OPTIONS;
    }
    else if (method_string_uppercase == "TRACE") {
        return HTTPMethod::TRACE;
    }
    else if (method_string_uppercase == "PATCH") {
        return HTTPMethod::PATCH;
    }
    else {
        throw std::invalid_argument("Unexpected HTTP method");
    }
}

std::string to_string(const HTTPRequest request) {
    std::ostringstream oss;
    
    oss << to_string(request.getMethod()) << ' ';
    oss << request.getUri().getPath() << ' ';
    oss << "HTTP/1.1" << "\r\n";
    for (const auto& header : request.getHeaders()) {
        oss << header.first << ": " << header.second << "\r\n";
    }
    oss << "\r\n";
    oss << request.getContent();
    
    return oss.str();
}

std::string to_string(const HTTPResponse response) {
    std::ostringstream oss;
    
    oss << "HTTP/1.1" << ' ';
    oss << static_cast<int>(response.getStatusCode()) << ' ';
    oss << to_string(response.getStatusCode()) << "\r\n";
    for (const auto& header : response.getHeaders()) {
        oss << header.first << ": " << header.second << "\r\n";
    }
    oss << "\r\n";
    oss << response.getContent();
    
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
