#include "stdafx.h"
#include "HttpResponseMessage.h"


// Public construction/destruction
Network::HttpResponseMessage::HttpResponseMessage()
{
    
}
Network::HttpResponseMessage::HttpResponseMessage(const std::string& Method, const std::string& URL)
{
    
}
Network::HttpResponseMessage::~HttpResponseMessage()
{
    if(mBody)
    {
        delete mBody;
        mBody = NULL;
    }
}


std::string Network::HttpResponseMessage::GetRequestHeader(const std::string& HeaderKey) const
{
    return std::string();
}
std::string Network::HttpResponseMessage::GetStringContentBody() const
{
    std::string ResponseContent;
    size_t ContentLength = mBody->GetContentLength();
    char* Content = new char[ContentLength];
    std::memset(Content, 0, ContentLength * sizeof(char));
    
    ResponseContent = std::string(Content);
    delete[] Content;
    return ResponseContent;
}
std::istream* Network::HttpResponseMessage::GetStreamContentBody() const
{
    return mBody->GetContent();
}

void Network::HttpResponseMessage::SetHeader(const std::string& Key, const std::string& Value)
{
    mHeaders[Key] = Value;
}
void Network::HttpResponseMessage::SetStringContent(const HttpStringContent* const Content)
{
    mBody = new HttpStringContent(*Content);
}
void Network::HttpResponseMessage::SetStringContent(const HttpStringContent& Content)
{
    mBody = new HttpStringContent(Content);
}
