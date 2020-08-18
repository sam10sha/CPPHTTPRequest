#include "stdafx.h"
#include "HttpByteContent.h"

// Public construction/destruction
Network::HttpByteContent::HttpByteContent() :
    mContentBytes(nullptr),
	mContentStreamBuf(nullptr)
{

}
Network::HttpByteContent::HttpByteContent(const void* const Content, const size_t ContentLength) :
	HttpContent(nullptr, 0),
    mContentBytes(new char[ContentLength]),
	mContentStreamBuf(new GenericIO::GenericStreamBuf(mContentBytes, ContentLength))
{
	mContentStreamBuf->sputn((const char*)Content, ContentLength);
    mContentStream = new std::istream(mContentStreamBuf);
    mContentLength = ContentLength;
}
Network::HttpByteContent::HttpByteContent(const HttpByteContent& Content) :
	HttpContent(nullptr, 0),
    mContentBytes(new char[Content.mContentLength]),
	mContentStreamBuf(new GenericIO::GenericStreamBuf(mContentBytes, Content.mContentLength))
{
	mContentStreamBuf->sputn((const char*)Content.mContentBytes, Content.mContentLength);
    mContentStream = new std::istream(mContentStreamBuf);
    mContentLength = Content.mContentLength;
}
Network::HttpByteContent::~HttpByteContent()
{
	if(mContentStreamBuf)
	{
		delete mContentStreamBuf;
		mContentStreamBuf = nullptr;
	}
    if(mContentBytes)
    {
        delete[] mContentBytes;
        mContentBytes = nullptr;
    }
}

// Public operator overloads
Network::HttpByteContent& Network::HttpByteContent::operator=(const HttpByteContent& Content)
{
    if(mContentStream)
    {
        delete mContentStream;
        mContentStream = nullptr;
    }
    if(mContentStreamBuf)
    {
    	delete mContentStreamBuf;
    	mContentStreamBuf = nullptr;
    }
    if(mContentBytes)
    {
        delete[] mContentBytes;
        mContentBytes = nullptr;
    }
    mContentLength = Content.mContentLength;
    mContentBytes = new char[Content.mContentLength];
    mContentStreamBuf = new GenericIO::GenericStreamBuf(mContentBytes, mContentLength);
	mContentStreamBuf->sputn((const char*)Content.mContentBytes, Content.mContentLength);
    mContentStream = new std::istream(mContentStreamBuf);
    return *this;
}
