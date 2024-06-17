//
//  http_message_interface.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 16/06/24.
//

#ifndef http_message_interface_hpp
#define http_message_interface_hpp

#include <stdio.h>
#include <string>
#include <map>

namespace http {

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

}

#endif /* http_message_interface_hpp */
