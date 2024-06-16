//
//  http_response.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#ifndef http_response_hpp
#define http_response_hpp

#include <stdio.h>
#include "http_message_interface.hpp"
#include "http_enums.hpp"

namespace http {

class HTTPResponse : public HTTPMessageInterface {
public:
    HTTPResponse();
    HTTPResponse(HTTPStatusCode status_code);
    ~HTTPResponse();
    
    void setStatusCode(HTTPStatusCode status_code);
    std::string toString() const;
    
    HTTPStatusCode getStatusCode() const;
    
private:
    HTTPStatusCode status_code_;
};

}

#endif /* http_response_hpp */
