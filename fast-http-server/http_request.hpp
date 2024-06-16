//
//  http_request.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#ifndef http_request_hpp
#define http_request_hpp

#include <stdio.h>
#include "http_message_interface.hpp"
#include "uri.hpp"
#include "http_enums.hpp"

namespace http {

class HTTPRequest : public HTTPMessageInterface {
public:
    HTTPRequest();
    ~HTTPRequest();
    
    void setMethod(HTTPMethod method);
    void setUri(const Uri& uri);
    
    HTTPMethod getMethod() const;
    Uri getUri() const;
    std::string toString() const;
    
private:
    std::string version_;
    HTTPMethod method_;
    Uri uri_;
};

HTTPRequest string_to_request(const std::string& request_string);

}

#endif /* http_request_hpp */
