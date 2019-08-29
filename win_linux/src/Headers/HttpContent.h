#ifndef HTTPCONTENT_H
#define HTTPCONTENT_H

#include "IStreamWrap.h"

namespace Network
{
    class HttpContent
    {
        // Private instance variables
        protected:
            std::istream* mContentStream;
            size_t mContentLength;
            
        // Protected construction
        protected:
            HttpContent();
            HttpContent(std::istream* const Content, const size_t ContentLength);
            HttpContent(const HttpContent& Content) = delete;
        // Public destruction
        public:
            virtual ~HttpContent();
        
        // Public operator overloads
        public:
            void operator=(const HttpContent& Content) = delete;
            
        // Public methods            
        public:
            virtual std::istream* GetContent();
            virtual void GetContent(IStreamWrap& Stream) const;
            virtual size_t GetContentLength() const;
    };
}

#endif // HTTPCONTENT_H

