#include "stdafx.h"
#include "HttpResponseMessage.h"


// Public construction/destruction
Network::HttpResponseMessage::HttpResponseMessage() :
    mStatusCode(0), mPort(0), mBody(NULL)
{
    
}
Network::HttpResponseMessage::HttpResponseMessage(const std::string& Method, const std::string& URL) :
    mStatusCode(0), mPort(0), mBody(NULL)
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


// Public methods
long Network::HttpResponseMessage::GetResponseStatusCode()
{
    return mStatusCode;
}
std::vector<std::string> Network::HttpResponseMessage::GetResponseHeader(const std::string& HeaderKey) const
{
    std::vector<std::string> HeaderValues;
    std::pair<std::multimap<const std::string, std::string>::const_iterator, std::multimap<const std::string, std::string>::const_iterator> RangeOfElements;
    RangeOfElements = mHeaders.equal_range(HeaderKey);
    for(std::multimap<const std::string, std::string>::const_iterator HeaderIndex = RangeOfElements.first; HeaderIndex != RangeOfElements.second; HeaderIndex++)
    {
        HeaderValues.push_back(HeaderIndex->second);
    }
    return HeaderValues;
}
std::string Network::HttpResponseMessage::GetStringContentBody() const
{
    std::string ResponseContent;
    if(mBody)
    {
        IStreamWrap StreamWrap;
        std::ostringstream ResponseContentOutStream;
        
        mBody->GetContent(StreamWrap);
        *(StreamWrap.mStream)>>ResponseContentOutStream.rdbuf();
        ResponseContent = ResponseContentOutStream.str();
    }
    return ResponseContent;
}
void Network::HttpResponseMessage::GetStreamContentBody(Network::IStreamWrap& StreamWrap) const
{
    mBody->GetContent(StreamWrap);
}

void Network::HttpResponseMessage::SetMethod(const std::string& Method)
{
    mMethod = Method;
}
void Network::HttpResponseMessage::SetURL(const std::string& URL)
{
    mURL = URL;
}
void Network::HttpResponseMessage::SetServerHostName(const std::string& ServerHostName)
{
    mServerHostName = ServerHostName;
}
void Network::HttpResponseMessage::SetServerIPAddr(const std::string& ServerIPAddr)
{
    mServerIPAddress = ServerIPAddr;
}
void Network::HttpResponseMessage::SetPort(const long Port)
{
    mPort = Port;
}
void Network::HttpResponseMessage::SetQueryPath(const std::string& QueryPath)
{
    mQueryPath = QueryPath;
}
void Network::HttpResponseMessage::SetHeader(const std::string& Key, const std::string& Value)
{
    mHeaders.insert(std::pair<const std::string, std::string>(Key, Value));
}
void Network::HttpResponseMessage::ParseRawHeader(const std::string& Header)
{
    std::vector<std::string> Lines;
    size_t HeaderLineDelimiterPosition;
    std::string HeaderKey;
    std::string HeaderValue;
    
    boost::split(Lines, Header, boost::is_any_of("\r\n"));
    
    // Parse status row
    if(Lines.size() > 0)
    {
        ParseResponseHeaderStatusLine(Lines[0]);
    }
    
    // Parse rest of headers
    for(std::vector<std::string>::iterator Line = Lines.begin() + 1; Line < Lines.end(); Line++)
    {
        HeaderLineDelimiterPosition = Line->find_first_of(':');
        if(HeaderLineDelimiterPosition != std::string::npos)
        {
            HeaderKey = Line->substr(0, HeaderLineDelimiterPosition);
            HeaderValue = Line->substr(HeaderLineDelimiterPosition + 1);
            mHeaders.insert(std::pair<const std::string, std::string>(HeaderKey, HeaderValue));
        }
    }
}
void Network::HttpResponseMessage::SetStringContent(const HttpStringContent* const Content)
{
    mBody = new HttpStringContent(*Content);
}
void Network::HttpResponseMessage::SetStringContent(const HttpStringContent& Content)
{
    mBody = new HttpStringContent(Content);
}


// Private methods
void Network::HttpResponseMessage::ParseResponseHeaderStatusLine(const std::string& HeaderStatusLine)
{
    std::vector<std::string> StatusLineComponents;
    std::vector<std::string> ProtocolWithVersionComponents;
    std::string StrStatusCode;
    
    // Parse status line
    boost::split(StatusLineComponents, HeaderStatusLine, boost::is_any_of(" "));
    if(StatusLineComponents.size() >= 3)
    {
        boost::split(ProtocolWithVersionComponents, StatusLineComponents[0], boost::is_any_of("/"));
        StrStatusCode.append(StatusLineComponents[1]);
        mStatusMessage.append(StatusLineComponents[2]);
    }
    
    // Parse protocl and version
    if(ProtocolWithVersionComponents.size() >= 2)
    {
        mProtocol.clear();
        mProtocolVersion.clear();
        
        mProtocol.append(ProtocolWithVersionComponents[0]);
        mProtocolVersion.append(ProtocolWithVersionComponents[1]);
    }
    
    // Parse status code
	std::stringstream StatusCodeStream(StrStatusCode);
	StatusCodeStream >> mStatusCode;
}
