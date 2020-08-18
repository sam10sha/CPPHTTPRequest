#ifndef HTTPBYTECONTENT_H
#define HTTPBYTECONTENT_H

#include "HttpContent.h"
#include "GenericStreamBuf.h"

namespace Network
{
    class HttpByteContent : public HttpContent
    {
    // Private embedded declarations
    private:
        /* class membuf : public std::basic_streambuf<char>
        {
		public:
			membuf(const char *p, size_t l)
			{
				setg((char*)p, (char*)p, (char*)p + l);
			}
        };
        class memstream : public std::istream
        {
        public:
            memstream(const char *p, size_t l) :
                std::istream(&_buffer),
                _buffer(p, l)
            {
                rdbuf(&_buffer);
            }

        private:
            membuf _buffer;
        }; */
        
        
        
    // Private instance variables
    private:
        char* mContentBytes;
        GenericIO::GenericStreamBuf* mContentStreamBuf;
        
    // Public construction/destruction
    public:
        HttpByteContent();
        HttpByteContent(const void* const Content, const size_t ContentLength);
        HttpByteContent(const HttpByteContent& Content);
        ~HttpByteContent();
        
    // Public operator overloads
    public:
        HttpByteContent& operator=(const HttpByteContent& Content);
    };
}

#endif // HTTPBYTECONTENT_H
