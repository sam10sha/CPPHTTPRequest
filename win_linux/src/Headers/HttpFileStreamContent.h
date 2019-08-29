#ifndef HTTPFILESTREAMCONTENT_H
#define HTTPFILESTREAMCONTENT_H

#include "HttpContent.h"
#include "IStreamWrap.h"

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
        
    // Public operator overloads
    public:
        void operator=(const HttpFileStreamContent& Content);
            
    // Public methods            
    public:
        std::istream* GetContent();
        void GetContent(IStreamWrap& Stream) const;
    };
}

#endif // HTTPFILESTREAMCONTENT_H
