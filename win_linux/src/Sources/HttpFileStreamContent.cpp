#include "stdafx.h"
#include "HttpFileStreamContent.h"

// Public construction/destruction
Network::HttpFileStreamContent::HttpFileStreamContent() { }
Network::HttpFileStreamContent::HttpFileStreamContent(const std::string& FileName) :
    mFileName(FileName)
{
    mContentStream = new std::ifstream(FileName.c_str());
    mContentStream->seekg(0, mContentStream->end);
    mContentLength = mContentStream->tellg();
    mContentStream->seekg(0, mContentStream->beg);
}
Network::HttpFileStreamContent::HttpFileStreamContent(const Network::HttpFileStreamContent& Content) :
    mFileName(Content.mFileName)
{
    mContentStream = new std::ifstream(Content.mFileName.c_str());
    mContentStream->seekg(0, mContentStream->end);
    mContentLength = mContentStream->tellg();
    mContentStream->seekg(0, mContentStream->beg);
}


// Public operator overloads
void Network::HttpFileStreamContent::operator=(const HttpFileStreamContent& Content)
{
    
}


// Public methods      
std::istream* Network::HttpFileStreamContent::GetContent()
{
    if(mContentStream)
    {
        delete mContentStream;
    }
    mContentStream = new std::ifstream(mFileName.c_str());
    return mContentStream;
} 
void Network::HttpFileStreamContent::GetContent(Network::IStreamWrap& Stream) const
{
    Stream.mStream = new std::ifstream(mFileName.c_str());
}