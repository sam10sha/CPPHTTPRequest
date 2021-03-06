#include "stdafx.h"
#include "HttpRequestMessage.h"


// Public construction
Network::HttpRequestMessage::HttpRequestMessage() :
    mDefaultProtocol("http"),
    mBody(NULL)
{
	
}
Network::HttpRequestMessage::HttpRequestMessage(const std::string& Method, const std::string& URL) :
    mMethod(Method),
    mURL(URL),
    mDefaultProtocol("http"),
    mBody(NULL)
{
    DecodeURL();
}
Network::HttpRequestMessage::~HttpRequestMessage()
{
    if(mBody)
    {
        delete mBody;
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
std::string Network::HttpRequestMessage::GetServerIPAddress() const
{
    return mServerIPAddress;
}
long Network::HttpRequestMessage::GetPort() const
{
    return mPort;
}
std::string Network::HttpRequestMessage::GetQueryPath() const
{
    return mQueryPath;
}
std::string Network::HttpRequestMessage::GetRequestHeader() const
{
    std::string Header;
    Header += GetMethod() + " " + GetQueryPath() + " HTTP/1.1\r\n";
    Header += "Host: " + GetServerHostName() + "\r\n";
    for(std::map<const std::string, std::string>::const_iterator HeaderEntry = mHeaders.begin();
        HeaderEntry != mHeaders.end();
        HeaderEntry++)
    {
        Header += HeaderEntry->first + ": " + HeaderEntry->second + "\r\n";
    }
    return Header;
}
std::string Network::HttpRequestMessage::GetRequestBodyString() const
{
    const size_t BufSize = 1 << 12; // 4 KB
    std::istream* ContentStream = NULL;
    size_t ContentLength = 0;
    char* Buffer = NULL;
    std::string RequestBody;
    
    if(mBody)
    {
        ContentStream = mBody->GetContent();
        ContentLength = mBody->GetContentLength();
        
        if(ContentLength >= BufSize)
        {
            Buffer = new char[BufSize];
            while(ContentStream->eof() == false)
            {
                std::memset(Buffer, 0, BufSize);
                ContentStream->read(Buffer, BufSize);
                RequestBody += Buffer;
            }
        }
        else
        {
            Buffer = new char[ContentLength];
            std::memset(Buffer, 0, ContentLength);
            ContentStream->read(Buffer, ContentLength);
            RequestBody += Buffer;
        }
        delete[] Buffer;
    }
    return RequestBody;
}
std::string Network::HttpRequestMessage::GetRequest() const
{
    std::string Request;
    std::string RequestBody = GetRequestBodyString();
    
    Request += GetRequestHeader();
    Request += "\r\n";
    if(RequestBody.length() > 0)
    {
        Request += GetRequestBodyString();
        Request += "\r\n";
    }
    return Request;
}


void Network::HttpRequestMessage::SetMethod(const std::string& Method)
{
    mMethod = Method;
}
void Network::HttpRequestMessage::SetURL(const std::string& URL)
{
    mURL = URL;
    DecodeURL();
}
void Network::HttpRequestMessage::SetHeader(const std::string& Key, const std::string& Value)
{
    mHeaders[Key] = Value;
}
void Network::HttpRequestMessage::SetByteContent(const HttpByteContent* const Content)
{
    mBody = new HttpByteContent(*Content);
}
void Network::HttpRequestMessage::SetByteContent(const HttpByteContent& Content)
{
    mBody = new HttpByteContent(Content);
}
void Network::HttpRequestMessage::SetStringContent(const HttpStringContent* const Content)
{
    mBody = new HttpStringContent(*Content);
}
void Network::HttpRequestMessage::SetStringContent(const HttpStringContent& Content)
{
    mBody = new HttpStringContent(Content);
}
void Network::HttpRequestMessage::SetStreamContent(const HttpFileStreamContent* const Content)
{
    mBody = new HttpFileStreamContent(*Content);
}
void Network::HttpRequestMessage::SetStreamContent(const HttpFileStreamContent& Content)
{
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
        mPort = strtol(URLCopy.c_str(), NULL, 10);
    }
    else
    {
        mServerHostName = URLCopy;
        mPort = -1;
    }
    SetRemoteServerIPAddr();
}
void Network::HttpRequestMessage::SetRemoteServerIPAddr()
{
    std::vector<IPEndPoint> IPAddrs;
    ResolveIPAddrs(IPAddrs);
    if(IPAddrs.size() > 0)
    {
        IPEndPoint& SelectedEndPoint = IPAddrs.back();
        mServerIPAddress = SelectedEndPoint.mIPAddr;
        if(mPort < 0)
        {
            mPort = SelectedEndPoint.mPort;
        }
    }
}
bool Network::HttpRequestMessage::ResolveIPAddrs(std::vector<Network::HttpRequestMessage::IPEndPoint>& IPAddrs)
{
    bool IPAddrsResolved = false;
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::resolver Resolver(IOService);
    boost::asio::ip::tcp::resolver::query Query(mServerHostName, mProtocol);
    try
    {
        for(boost::asio::ip::tcp::resolver::iterator Iterator = Resolver.resolve(Query);
            Iterator != boost::asio::ip::tcp::resolver::iterator();
            Iterator++)
        {
            boost::asio::ip::tcp::endpoint EndPt = *Iterator;
            IPAddrs.push_back(
                IPEndPoint(
                    EndPt.address().to_string(),
                    EndPt.port()
                )
            );
        }
        IPAddrsResolved = true;
    }
    catch(boost::wrapexcept<boost::system::system_error> error)
    {
        std::cout << error.what() << std::endl;
    }
    return IPAddrsResolved;
}


