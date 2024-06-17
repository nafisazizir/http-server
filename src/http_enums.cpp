//
//  http_enums.cpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#include "http_enums.hpp"
#include <string>

namespace http {

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

}
