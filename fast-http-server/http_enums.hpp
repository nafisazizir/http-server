//
//  http_enums.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#ifndef http_enums_hpp
#define http_enums_hpp

#include <stdio.h>
#include <string>

namespace http {

enum class HTTPMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH
};

enum class HTTPStatusCode {
    Continue = 100,
    SwitchingProtocols = 101,
    EarlyHints = 103,
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritativeInformation = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestTimeout = 408,
    ImATeapot = 418,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnvailable = 503,
    GatewayTimeout = 504,
    HttpVersionNotSupported = 505
};

std::string to_string(HTTPMethod method);
std::string to_string(HTTPStatusCode status_code);
HTTPMethod string_to_method(const std::string& method_string);

}

#endif /* http_enums_hpp */
