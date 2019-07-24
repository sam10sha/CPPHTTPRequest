#include "stdafx.h"
#include "HttpStringContent.h"

// Public construction/destruction
Network::HttpStringContent::HttpStringContent() { }
Network::HttpStringContent::HttpStringContent(const std::string& Content) :
    mStringContent(Content)
{
    mContentStream = new std::istringstream(mStringContent);
    mContentLength = Content.size();
}
Network::HttpStringContent::HttpStringContent(const HttpStringContent& Content) :
    mStringContent(Content.mStringContent)
{
    mContentStream = new std::istringstream(mStringContent);
    mContentLength = Content.mContentLength;
}
Network::HttpStringContent::~HttpStringContent() { }

// Public operator overloads
Network::HttpStringContent& Network::HttpStringContent::operator=(const HttpStringContent& Content)
{
    if(mContentStream)
    {
        delete mContentStream;
        mContentStream = NULL;
    }
    mContentStream = new std::istringstream(mStringContent);
    mContentLength = Content.mContentLength;
    return *this;
}
