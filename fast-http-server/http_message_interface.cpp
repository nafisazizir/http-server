//
//  http_message_interface.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#include "http_message_interface.hpp"

namespace http {

HTTPMessageInterface::HTTPMessageInterface() {}
HTTPMessageInterface::~HTTPMessageInterface() {}

void HTTPMessageInterface::setHeader(const std::string& key, const std::string& value) {
    headers_[key] = std::move(value);
}

void HTTPMessageInterface::removeHeader(const std::string& key) {
    headers_.erase(key);
}

void HTTPMessageInterface::clearHeader() {
    headers_.clear();
}

void HTTPMessageInterface::setContent(const std::string& content) {
    content_ = std::move(content);
    setContentLength();
}

void HTTPMessageInterface::clearContent() {
    content_.clear();
}

std::string HTTPMessageInterface::getHeader(const std::string& key) const {
    if (headers_.count(key) > 0) {
        return headers_.at(key);
    }
    return std::string();
}

std::map<std::string, std::string> HTTPMessageInterface::getHeaders() const {
    return headers_;
}

std::string HTTPMessageInterface::getContent() const {
    return content_;
}

size_t HTTPMessageInterface::getContentLength() const {
    return content_.length();
}

void HTTPMessageInterface::setContentLength() {
    setHeader("Content-Length", std::to_string(content_.length()));
}


}
