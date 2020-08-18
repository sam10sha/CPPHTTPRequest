#include "stdafx.h"
#include "HttpFileStreamContent.h"

// Public construction/destruction
Network::HttpFileStreamContent::HttpFileStreamContent() { }
Network::HttpFileStreamContent::HttpFileStreamContent(const std::string& FileName) :
	HttpContent(new std::ifstream(FileName.c_str()), 0),
    mFileName(FileName)
{
    mContentStream->seekg(0, mContentStream->end);
    mContentLength = mContentStream->tellg();
    mContentStream->seekg(0, mContentStream->beg);
}
Network::HttpFileStreamContent::HttpFileStreamContent(const Network::HttpFileStreamContent& Content) :
	HttpContent(new std::ifstream(Content.mFileName.c_str()), Content.mContentLength),
    mFileName(Content.mFileName)
{

}
Network::HttpFileStreamContent::~HttpFileStreamContent()
{
	((std::ifstream*)mContentStream)->close();
}


// Public operator overloads
Network::HttpFileStreamContent& Network::HttpFileStreamContent::operator=(const HttpFileStreamContent& Content)
{
	mFileName = Content.mFileName;
	if(mContentStream)
	{
		((std::ifstream*)mContentStream)->close();
		delete mContentStream;
		mContentStream = nullptr;
	}
    mContentStream = new std::ifstream(mFileName.c_str());
    mContentStream->seekg(0, mContentStream->end);
    mContentLength = mContentStream->tellg();
    mContentStream->seekg(0, mContentStream->beg);
    return *this;
}
