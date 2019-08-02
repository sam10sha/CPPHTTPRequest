#include "stdafx.h"
#include "HttpContent.h"

// Protected construction
Network::HttpContent::HttpContent() :
    mContentStream(new std::istringstream(std::string())),
    mContentLength(0) { }
Network::HttpContent::HttpContent(std::istream* const Content, const size_t ContentLength) :
    mContentStream(Content),
    mContentLength(ContentLength)
{
    if(mContentStream == NULL)
    {
        mContentStream = new std::istringstream(std::string());
        mContentLength = 0;
    }
}
/* Network::HttpContent::HttpContent(const HttpContent& Content) :
    mContentStream(NULL),
    mContentLength(Content.mContentLength) { } */

// Public destruction
Network::HttpContent::~HttpContent()
{
    if(mContentStream)
    {
        delete mContentStream;
        mContentStream = NULL;
    }
}


// Public methods      
std::istream* Network::HttpContent::GetContent()
{
    return mContentStream;
}  
size_t Network::HttpContent::GetContentLength() const
{
    return mContentLength;
}

