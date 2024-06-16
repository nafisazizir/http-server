//
//  http_message.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#ifndef http_message_hpp
#define http_message_hpp

#include <stdio.h>
#include <string>
#include <map>
#include "uri.hpp"

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

// Utility functions to convert between string/int to enum classes
std::string to_string(HTTPMethod method);
std::string to_string(HTTPStatusCode status_code);
HTTPMethod string_to_method(const std::string& method_string);

class HTTPMessageInterface {
public:
    HTTPMessageInterface();
    virtual ~HTTPMessageInterface();
    
    void setHeader(const std::string& key, const std::string& value);
    void removeHeader(const std::string& key);
    void clearHeader();
    void setContent(const std::string& content);
    void clearContent();
    
    std::string getHeader(const std::string& key) const;
    std::map<std::string, std::string> getHeaders() const;
    std::string getContent() const;
    size_t getContentLength() const;
    
protected:
    std::map<std::string, std::string> headers_;
    std::string content_;
    
    void setContentLength();
};

class HTTPRequest : public HTTPMessageInterface {
public:
    HTTPRequest();
    ~HTTPRequest();
    
    void setMethod(HTTPMethod method);
    void setUri(const Uri& uri);
    
    HTTPMethod getMethod() const;
    Uri getUri() const;
    
private:
    std::string version_;
    HTTPMethod method_;
    Uri uri_;
};

class HTTPResponse : public HTTPMessageInterface {
public:
    HTTPResponse();
    HTTPResponse(HTTPStatusCode status_code);
    ~HTTPResponse();
    
    void setStatusCode(HTTPStatusCode status_code);
    std::string toString();
    
    HTTPStatusCode getStatusCode() const;
    
private:
    HTTPStatusCode status_code_;
};

// Utility function to convert HTTP message objects to string and vice versa
std::string to_string(const HTTPRequest& request);
std::string to_string(const HTTPResponse& response);
HTTPRequest string_to_request(const std::string& request_string);
//HTTPResponse string_to_response(const std::string& response_string);

}

#endif /* http_message_hpp */
