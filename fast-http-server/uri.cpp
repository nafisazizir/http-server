//
//  uri.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#include "uri.hpp"

namespace http {

http::Uri::Uri() : path_("/") {}
http::Uri::~Uri() {}

http::Uri::Uri(const std::string& path) : path_(path) {
    setPathToLowercase();
}

void http::Uri::setPath(const std::string& path) {
    path_ = std::move(path);
    setPathToLowercase();
}

std::string http::Uri::getScheme() const {
    return scheme_;
}

std::string http::Uri::getHost() const {
    return host_;
}

std::uint16_t http::Uri::getPort() const {
    return port_;
}

std::string http::Uri::getPath() const {
    return path_;
}

void http::Uri::setPathToLowercase() {
    std::transform(path_.begin(), path_.end(), path_.begin(), [](char c) {return _tolower(c); });
}
}
