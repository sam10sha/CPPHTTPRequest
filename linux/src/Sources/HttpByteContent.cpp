#include "stdafx.h"
#include "HttpByteContent.h"

// Public construction/destruction
Network::HttpByteContent::HttpByteContent() :
    mContentBytes(NULL) { }
Network::HttpByteContent::HttpByteContent(const void* const Content, const size_t ContentLength) :
    mContentBytes(new char[ContentLength])
{
    memcpy(mContentBytes, (const char*)Content, ContentLength);
    //mContentStream = new memstream(mContentBytes, ContentLength);
    mContentLength = ContentLength;
}
Network::HttpByteContent::HttpByteContent(const HttpByteContent& Content) :
    mContentBytes(new char[Content.mContentLength])
{
    memcpy(mContentBytes, Content.mContentBytes, Content.mContentLength);
    //mContentStream = new memstream(mContentBytes, Content.mContentLength);
    mContentLength = Content.mContentLength;
}
Network::HttpByteContent::~HttpByteContent()
{
    if(mContentBytes)
    {
        delete[] mContentBytes;
        mContentBytes = NULL;
    }
}

// Public operator overloads
void Network::HttpByteContent::operator=(HttpByteContent& Content)
{
    if(mContentBytes)
    {
        delete[] mContentBytes;
        mContentBytes = NULL;
    }
    mContentBytes = new char[Content.mContentLength];
    memcpy(mContentBytes, Content.GetContent(), Content.GetContentLength());
    
    if(mContentStream)
    {
        delete mContentStream;
        mContentStream = NULL;
    }
    mContentStream = new std::istringstream(std::string((char*)mContentBytes));
    mContentLength = Content.mContentLength;
}


// Public member functions
std::istream* Network::HttpByteContent::GetContent()
{
    if(mContentStream)
    {
        delete mContentStream;
    }
    mContentStream = new memstream(mContentBytes, mContentLength);
    return mContentStream;
}
