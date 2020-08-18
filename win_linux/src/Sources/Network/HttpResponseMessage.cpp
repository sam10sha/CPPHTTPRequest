#include "stdafx.h"
#include "HttpResponseMessage.h"


// Public construction/destruction
Network::HttpResponseMessage::HttpResponseMessage() :
    mStatusCode(0), mPort(0), mBody(nullptr), BodyManagedExternally(false)
{
    
}
Network::HttpResponseMessage::HttpResponseMessage(const std::string& Method, const std::string& URL) :
    mStatusCode(0), mPort(0), mBody(nullptr), BodyManagedExternally(false)
{
    
}
Network::HttpResponseMessage::~HttpResponseMessage()
{
    if(!BodyManagedExternally && mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
}


// Public methods
long Network::HttpResponseMessage::GetResponseStatusCode() const
{
    return mStatusCode;
}
std::string Network::HttpResponseMessage::GetResponseStatusMessage() const
{
	return mStatusMessage;
}
std::multimap<const std::string, std::string> Network::HttpResponseMessage::GetAllHeaders() const
{
	std::multimap<const std::string, std::string> AllHeaders(mMultiHeaders);
	for(std::map<const std::string, std::string>::const_iterator SingleHeaderEntry = mSingleHeaders.begin();
		SingleHeaderEntry != mSingleHeaders.end();
		SingleHeaderEntry++)
	{
		AllHeaders.insert(std::pair<const std::string, std::string>(SingleHeaderEntry->first, SingleHeaderEntry->second));
	}
	return AllHeaders;
}
std::vector<std::string> Network::HttpResponseMessage::GetResponseHeader(const std::string& HeaderKey) const
{
    std::vector<std::string> HeaderValues;

    std::map<const std::string, std::string>::const_iterator SingleHeader = mSingleHeaders.find(HeaderKey);
    if(SingleHeader != mSingleHeaders.end())
    {
    	HeaderValues.push_back(SingleHeader->second);
    }

    std::pair<std::multimap<const std::string, std::string>::const_iterator, std::multimap<const std::string, std::string>::const_iterator> RangeOfElements;
    RangeOfElements = mMultiHeaders.equal_range(HeaderKey);
    for(std::multimap<const std::string, std::string>::const_iterator MultiHeaderIndex = RangeOfElements.first; MultiHeaderIndex != RangeOfElements.second; MultiHeaderIndex++)
    {
        HeaderValues.push_back(MultiHeaderIndex->second);
    }
    return HeaderValues;
}
std::string Network::HttpResponseMessage::GetStringContentBody() const
{
    std::string ResponseContent;
    if(mBody)
    {
    	std::ostringstream ResponseContentOutStream;
    	std::istream& ContentStream = *mBody->GetContent();
    	ContentStream >> ResponseContentOutStream.rdbuf();
    	ResponseContent = ResponseContentOutStream.str();
    }
    return ResponseContent;
}
std::istream* Network::HttpResponseMessage::GetStreamContentBody() const
{
	return mBody ? mBody->GetContent() : nullptr;
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
void Network::HttpResponseMessage::AddHeader(const std::string& Key, const std::string& Value)
{
	std::map<const std::string, std::string>::const_iterator SingleHeader = mSingleHeaders.find(Key);
	if(SingleHeader != mSingleHeaders.end())
	{
		mMultiHeaders.insert(std::pair<const std::string, std::string>(SingleHeader->first, SingleHeader->second));
		mSingleHeaders.erase(SingleHeader);
	}
	mMultiHeaders.insert(std::pair<const std::string, std::string>(Trim(Key), Trim(Value)));
}
void Network::HttpResponseMessage::SetHeader(const std::string& Key, const std::string& Value)
{
	std::pair<std::multimap<const std::string, std::string>::const_iterator, std::multimap<const std::string, std::string>::const_iterator> RangeOfElements;
	RangeOfElements = mMultiHeaders.equal_range(Key);
	if(RangeOfElements.first != RangeOfElements.second)
	{
		mMultiHeaders.erase(RangeOfElements.first, RangeOfElements.second);
	}
	mSingleHeaders[Trim(Key)] = Trim(Value);
}
void Network::HttpResponseMessage::ParseRawHeader(const std::string& Header)
{
    std::vector<std::string> Lines;
    size_t HeaderLineDelimiterPosition;
    std::string HeaderKey;
    std::string HeaderValue;
    std::map<const std::string, std::string>::iterator SingleIndex;
    std::pair< std::multimap<const std::string, std::string>::iterator, std::multimap<const std::string, std::string>::iterator > RangeOfElements;
    
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
            SingleIndex = mSingleHeaders.find(HeaderKey);
			RangeOfElements = mMultiHeaders.equal_range(HeaderKey);
			if(SingleIndex != mSingleHeaders.end())
			{
				// move from single header list to multi header list
				mMultiHeaders.insert(std::pair<const std::string, std::string>(SingleIndex->first, SingleIndex->second));
				mMultiHeaders.insert(std::pair<const std::string, std::string>(HeaderKey, HeaderValue));

				mSingleHeaders.erase(SingleIndex);
			}
			else if(RangeOfElements.first != RangeOfElements.second)
			{
				// push into multi header list
				mMultiHeaders.insert(std::pair<const std::string, std::string>(HeaderKey, HeaderValue));
			}
			else
			{
				// add to single header list
				mSingleHeaders[HeaderKey] = HeaderValue;
			}
            //mMultiHeaders.insert(std::pair<const std::string, std::string>(HeaderKey, HeaderValue));
        }
    }
}
void Network::HttpResponseMessage::SetStringContent(const HttpStringContent* const Content)
{
	if(!BodyManagedExternally && mBody)
	{
		delete mBody;
		mBody = nullptr;
	}
    mBody = new HttpStringContent(*Content);
    BodyManagedExternally = false;
}
void Network::HttpResponseMessage::SetStringContent(const HttpStringContent& Content)
{
	if(!BodyManagedExternally && mBody)
	{
		delete mBody;
		mBody = nullptr;
	}
    mBody = new HttpStringContent(Content);
    BodyManagedExternally = false;
}
void Network::HttpResponseMessage::SetContent(HttpContent* const Content, const bool IsManagedExternally)
{
	if(!BodyManagedExternally && mBody)
	{
		delete mBody;
		mBody = nullptr;
	}
	mBody = Content;
	BodyManagedExternally = IsManagedExternally;
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
        StrStatusCode = (StatusLineComponents[1]);
        //mStatusMessage = (StatusLineComponents[2]);
        mStatusMessage = boost::algorithm::join(
			std::vector<std::string>(StatusLineComponents.begin() + 2, StatusLineComponents.end()),
			" "
		);
    }
    
    // Parse protocol and version
    if(ProtocolWithVersionComponents.size() >= 2)
    {
        mProtocol.clear();
        mProtocolVersion.clear();
        
        mProtocol = (ProtocolWithVersionComponents[0]);
        mProtocolVersion = (ProtocolWithVersionComponents[1]);
    }
    
    // Parse status code
	std::stringstream StatusCodeStream(StrStatusCode);
	StatusCodeStream >> mStatusCode;
}
std::string Network::HttpResponseMessage::Trim(const std::string& Str) const
{
	const std::string WHITESPACE = " \n\r\t\f\v";
	std::string TrimmedStr(Str);
	size_t start = TrimmedStr.find_first_not_of(WHITESPACE);
	size_t end = TrimmedStr.find_last_not_of(WHITESPACE);
	TrimmedStr = (start == std::string::npos) ? "" : TrimmedStr.substr(start);
	TrimmedStr = (end == std::string::npos) ? "" : TrimmedStr.substr(0, end + 1);
	return TrimmedStr;
}
