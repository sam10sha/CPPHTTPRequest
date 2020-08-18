#include "stdafx.h"
#include "HttpRequestMessage.h"

// Public construction
Network::HttpRequestMessage::HttpRequestMessage() :
    mDefaultProtocol("http"),
    mServerPort(-1),
    mBody(nullptr)
{
    SetHeader("Connection", "Close");
}
Network::HttpRequestMessage::HttpRequestMessage(const std::string& Method, const std::string& URL) :
    mMethod(Method),
    mURL(URL),
    mDefaultProtocol("http"),
    mServerPort(-1),
    mBody(nullptr)
{
    DecodeURL();
    SetHeader("Host", GetServerHostName());
    SetHeader("Connection", "Close");
}
Network::HttpRequestMessage::~HttpRequestMessage()
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
}

// Public methods
std::string Network::HttpRequestMessage::GetMethod() const
{
    return mMethod;
}
std::string Network::HttpRequestMessage::GetURL() const
{
    return mURL;
}
std::string Network::HttpRequestMessage::GetProtocol() const
{
    return mProtocol;
}
std::string Network::HttpRequestMessage::GetServerHostName() const
{
    return mServerHostName;
}
const std::vector<Network::HttpRequestMessage::IPEndPoint>& Network::HttpRequestMessage::GetRemoteServerIPEndPts()
{
    return mServerIPEndPts;
}
std::string Network::HttpRequestMessage::GetServerIPAddr() const
{
    return mServerIPAddr;
}
long Network::HttpRequestMessage::GetPort() const
{
    return mServerPort;
}
std::string Network::HttpRequestMessage::GetQueryPath() const
{
    return mQueryPath;
}
std::vector<std::string> Network::HttpRequestMessage::GetRequestHeader(const std::string& Key) const
{
    std::vector<std::string> HeaderValues;

    std::map<const std::string, std::string>::const_iterator SingleHeader = mSingleHeaders.find(Key);
    if(SingleHeader != mSingleHeaders.end())
    {
    	HeaderValues.push_back(SingleHeader->second);
    }

    std::pair<std::multimap<const std::string, std::string>::const_iterator, std::multimap<const std::string, std::string>::const_iterator> RangeOfElements;
    RangeOfElements = mMultiHeaders.equal_range(Key);
    for(std::multimap<const std::string, std::string>::const_iterator MultiHeaderIndex = RangeOfElements.first; MultiHeaderIndex != RangeOfElements.second; MultiHeaderIndex++)
    {
        HeaderValues.push_back(MultiHeaderIndex->second);
    }
    return HeaderValues;
}
std::string Network::HttpRequestMessage::GetRequestHeaderSection() const
{
    std::string Header;
    Header += GetMethod() + " " + GetQueryPath() + " HTTP/1.1\r\n";
    //Header += "Host: " + GetServerHostName() + "\r\n";
    for(std::map<const std::string, std::string>::const_iterator SingleHeaderEntry = mSingleHeaders.begin();
		SingleHeaderEntry != mSingleHeaders.end();
		SingleHeaderEntry++)
    {
    	Header += SingleHeaderEntry->first + ": " + SingleHeaderEntry->second + "\r\n";
    }
    for(std::multimap<const std::string, std::string>::const_iterator MultiHeaderEntry = mMultiHeaders.begin();
        MultiHeaderEntry != mMultiHeaders.end();
        MultiHeaderEntry++)
    {
        Header += MultiHeaderEntry->first + ": " + MultiHeaderEntry->second + "\r\n";
    }
    return Header;
}
/* std::string Network::HttpRequestMessage::GetRequestBodyString() const
{
    const size_t BufSize = 1 << 12; // 4 KB
    std::istream* ContentStream = nullptr;
    size_t ContentLength = 0;
    char* Buffer = nullptr;
    std::string RequestBody;
    
    if(mBody)
    {
        ContentStream = mBody->GetContent();
        ContentLength = mBody->GetContentLength();
        
        if(ContentLength >= BufSize - 1)
        {
            Buffer = new char[BufSize];
            while(ContentStream->eof() == false)
            {
                std::memset(Buffer, 0, BufSize);
                ContentStream->read(Buffer, BufSize - 1);
                RequestBody += Buffer;
            }
        }
        else
        {
            Buffer = new char[ContentLength + 1];
            std::memset(Buffer, 0, ContentLength + 1);
            ContentStream->read(Buffer, (std::streamsize)ContentLength);
            RequestBody += Buffer;
        }
        delete[] Buffer;
        if(RequestBody.length() > 0)
        {
            RequestBody += "\r\n";
        }
    }
    return RequestBody;
} */
std::istream* Network::HttpRequestMessage::GetRequestBodyStream() const
{
	std::istream* ContentStream = nullptr;
	if(mBody)
	{
		ContentStream = mBody->GetContent();
	}
	return ContentStream;
}


void Network::HttpRequestMessage::SetMethod(const std::string& Method)
{
    mMethod = Method;
}
void Network::HttpRequestMessage::SetURL(const std::string& URL)
{
    mURL = URL;
    DecodeURL();
    SetHeader("Host", GetServerHostName());
}
void Network::HttpRequestMessage::AddRemoteServerEndPt(IPEndPoint IPEndPt)
{
    mServerIPEndPts.push_back(IPEndPt);
    
    /* std::vector<IPEndPoint> IPAddrs;
    ResolveIPAddrs(IPAddrs);
    if(IPAddrs.size() > 0)
    {
        IPEndPoint& SelectedEndPoint = IPAddrs.back();
        mServerIPAddress = SelectedEndPoint.mIPAddr;
        if(mServerPort < 0)
        {
            mServerPort = SelectedEndPoint.mServerPort;
        }
    } */
}
void Network::HttpRequestMessage::SetRemoteServerIPAddr(const std::string& IPAddr)
{
    mServerIPAddr = IPAddr;
}
void Network::HttpRequestMessage::SetRemoteServerPort(const long Port)
{
    mServerPort = Port;
}
void Network::HttpRequestMessage::AddHeader(const std::string& Key, const std::string& Value)
{
    std::map<const std::string, std::string>::const_iterator SingleHeader = mSingleHeaders.find(Key);
    if(SingleHeader != mSingleHeaders.end())
    {
    	mMultiHeaders.insert(std::pair<const std::string, std::string>(SingleHeader->first, SingleHeader->second));
    	mSingleHeaders.erase(SingleHeader);
    }
    mMultiHeaders.insert(std::pair<const std::string, std::string>(Trim(Key), Trim(Value)));
}
void Network::HttpRequestMessage::SetHeader(const std::string& Key, const std::string& Value)
{
	std::pair<std::multimap<const std::string, std::string>::const_iterator, std::multimap<const std::string, std::string>::const_iterator> RangeOfElements;
	RangeOfElements = mMultiHeaders.equal_range(Key);
	if(RangeOfElements.first != RangeOfElements.second)
	{
		mMultiHeaders.erase(RangeOfElements.first, RangeOfElements.second);
	}
	mSingleHeaders[Trim(Key)] = Trim(Value);
}
void Network::HttpRequestMessage::SetByteContent(const HttpByteContent* const Content)
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
    mBody = new HttpByteContent(*Content);
}
void Network::HttpRequestMessage::SetByteContent(const HttpByteContent& Content)
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
    mBody = new HttpByteContent(Content);
}
void Network::HttpRequestMessage::SetStringContent(const HttpStringContent* const Content)
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
    mBody = new HttpStringContent(*Content);
}
void Network::HttpRequestMessage::SetStringContent(const HttpStringContent& Content)
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
    mBody = new HttpStringContent(Content);
}
void Network::HttpRequestMessage::SetStreamContent(const HttpFileStreamContent* const Content)
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
    mBody = new HttpFileStreamContent(*Content);
}
void Network::HttpRequestMessage::SetStreamContent(const HttpFileStreamContent& Content)
{
    if(mBody)
    {
        delete mBody;
        mBody = nullptr;
    }
    mBody = new HttpFileStreamContent(Content);
}





// Private methods
void Network::HttpRequestMessage::DecodeURL()
{
    const std::string ProtocolDelimiter("://");
    const std::string QueryPathDelimiter("/");
    const std::string PortDelimiter(":");
    size_t DelimiterIndex;
    std::string URLCopy(mURL);
    mProtocol.clear();
    mServerHostName.clear();
    mQueryPath.clear();
    
    DelimiterIndex = URLCopy.find(ProtocolDelimiter);
    if(DelimiterIndex != std::string::npos)
    {
        mProtocol = URLCopy.substr(0, DelimiterIndex);
        URLCopy = URLCopy.substr(DelimiterIndex + ProtocolDelimiter.size());
    }
    else
    {
        mProtocol = mDefaultProtocol;
    }
    DelimiterIndex = URLCopy.find(QueryPathDelimiter);
    if(DelimiterIndex != std::string::npos)
    {
        mQueryPath = URLCopy.substr(DelimiterIndex);
        URLCopy = URLCopy.substr(0, DelimiterIndex);
    }
    else
    {
        mQueryPath = QueryPathDelimiter;
    }
    DelimiterIndex = URLCopy.find(PortDelimiter);
    if(DelimiterIndex != std::string::npos)
    {
        mServerHostName = URLCopy.substr(0, DelimiterIndex);
        URLCopy = URLCopy.substr(DelimiterIndex + PortDelimiter.size());
        mServerPort = strtol(URLCopy.c_str(), nullptr, 10);
    }
    else
    {
        mServerHostName = URLCopy;
        mServerPort = -1;
    }
    //SetRemoteServerIPAddr();
}
std::string Network::HttpRequestMessage::Trim(const std::string& Str) const
{
	const std::string WHITESPACE = " \n\r\t\f\v";
	std::string TrimmedStr(Str);
	size_t start = TrimmedStr.find_first_not_of(WHITESPACE);
	size_t end = TrimmedStr.find_last_not_of(WHITESPACE);
	TrimmedStr = (start == std::string::npos) ? "" : TrimmedStr.substr(start);
	TrimmedStr = (end == std::string::npos) ? "" : TrimmedStr.substr(0, end + 1);
	return TrimmedStr;
}


