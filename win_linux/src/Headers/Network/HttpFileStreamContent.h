#ifndef HTTPFILESTREAMCONTENT_H
#define HTTPFILESTREAMCONTENT_H

#include "HttpContent.h"

namespace Network
{
    class HttpFileStreamContent : public HttpContent
    {
    // Private instance variables
    private:
        std::string mFileName;
        
    // Public construction/destruction
    public:
        HttpFileStreamContent();
        HttpFileStreamContent(const std::string& FileName);
        HttpFileStreamContent(const HttpFileStreamContent& Content);
        ~HttpFileStreamContent();
        
    // Public operator overloads
    public:
        HttpFileStreamContent& operator=(const HttpFileStreamContent& Content);
    };
}

#endif // HTTPFILESTREAMCONTENT_H