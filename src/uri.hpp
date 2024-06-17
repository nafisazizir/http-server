//
//  uri.hpp
//  fast-http-server
//
//  Created by Nafis Azizi Riza on 15/06/24.
//

#ifndef uri_hpp
#define uri_hpp

#include <stdio.h>
#include <string>

namespace http {
class Uri {
public:
    Uri();
    Uri(const std::string& path);
    ~Uri();
    
    void setPath(const std::string& path);
    
    std::string getScheme() const;
    std::string getHost() const;
    std::uint16_t getPort() const;
    std::string getPath() const;
    
private:
    std::string scheme_;
    std::string host_;
    std::uint16_t port_;
    std::string path_;
    
    void setPathToLowercase();
};
}

#endif /* uri_hpp */
