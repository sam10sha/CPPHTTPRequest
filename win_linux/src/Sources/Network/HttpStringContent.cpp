#include "stdafx.h"
#include "HttpStringContent.h"

// Public construction/destruction
Network::HttpStringContent::HttpStringContent() { }
Network::HttpStringContent::HttpStringContent(const std::string& Content) :
	HttpContent(new std::istringstream(Content), Content.size()),
    mStringContent(Content)
{

}
Network::HttpStringContent::HttpStringContent(const HttpStringContent& Content) :
	HttpContent(new std::istringstream(Content.mStringContent), Content.mContentLength),
    mStringContent(Content.mStringContent)
{

}

// Public operator overloads
Network::HttpStringContent& Network::HttpStringContent::operator=(const HttpStringContent& Content)
{
	mStringContent = Content.mStringContent;
    if(mContentStream)
    {
        delete mContentStream;
        mContentStream = nullptr;
    }
    mContentStream = new std::istringstream(mStringContent);
    mContentLength = Content.mContentLength;
    return *this;
}
