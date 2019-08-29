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


// Public methods
std::istream* Network::HttpStringContent::GetContent()
{
    if(mContentStream)
    {
        delete mContentStream;
    }
    mContentStream = new std::istringstream(mStringContent);
    return mContentStream;
}
void Network::HttpStringContent::GetContent(Network::IStreamWrap& Stream) const
{
    Stream.mStream = new std::istringstream(mStringContent);
}
