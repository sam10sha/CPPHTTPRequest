#ifndef HTTPSTRINGCONTENT_H
#define HTTPSTRINGCONTENT_H

#include "HttpContent.h"
#include "IStreamWrap.h"

namespace Network
{
    class HttpStringContent : public HttpContent
    {
    // Private instance variables
    private:
        std::string mStringContent;
        
    // Public construction/destruction
    public:
        HttpStringContent();
        HttpStringContent(const std::string& Content);
        HttpStringContent(const HttpStringContent& Content);
        ~HttpStringContent();
        
    // Public operator overloads
    public:
        HttpStringContent& operator=(const HttpStringContent& Content);
            
	// Public methods            
	public:
		virtual std::istream* GetContent();
		virtual void GetContent(IStreamWrap& Stream) const;
    };
}

#endif // HTTPSTRINGCONTENT_H
