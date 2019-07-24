#include "stdafx.h"
#include "HttpRequestMessage.h"


// Public construction
Networking::HttpRequestMessage::HttpRequestMessage() :
	mDefaultProtocol("http")
{
	
}
Networking::HttpRequestMessage::HttpRequestMessage(const std::string& Method, const std::string& URL) :
	mDefaultProtocol("http"),
	mMethod(Method),
	mURL(URL)
{
	DecodeURL();
}


// Public methods
std::string Networking::HttpRequestMessage::GetMethod() const
{
	return mMethod;
}
std::string Networking::HttpRequestMessage::GetURL() const
{
	return mURL;
}
std::string Networking::HttpRequestMessage::GetProtocol() const
{
	return mProtocol;
}
std::string Networking::HttpRequestMessage::GetServerHostName() const
{
	return mServerHostName;
}
std::string Networking::HttpRequestMessage::GetServerIPAddress() const
{
	return mServerIPAddress;
}
long Networking::HttpRequestMessage::GetPort() const
{
	return mPort;
}
std::string Networking::HttpRequestMessage::GetQueryPath() const
{
	return mQueryPath;
}
std::string Networking::HttpRequestMessage::GetRequestHeader() const
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
std::string Networking::HttpRequestMessage::GetRequestBody() const
{
	return std::string(mBody);
}
std::string Networking::HttpRequestMessage::GetRequest() const
{
	std::string Request;
	Request += GetRequestHeader();
	Request += "\r\n";
	Request += GetRequestBody();
	return Request;
}


void Networking::HttpRequestMessage::SetMethod(const std::string& Method)
{
	mMethod = Method;
}
void Networking::HttpRequestMessage::SetURL(const std::string& URL)
{
	mURL = URL;
	DecodeURL();
}
void Networking::HttpRequestMessage::SetHeader(const std::string& Key, const std::string& Value)
{
	mHeaders[Key] = Value;
}
void Networking::HttpRequestMessage::SetBody(const std::string& Value)
{
	mBody = Value;
}





// Private methods
void Networking::HttpRequestMessage::DecodeURL()
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
}
void Networking::HttpRequestMessage::ResolveIPAddrs(std::vector<Networking::HttpRequestMessage::IPEndPoint> IPAddrs)
{
	bool IPAddrsResolved = false;
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::resolver resolver(IOService);
    boost::asio::ip::tcp::resolver::query query(address, protocol);
    
    try
    {
        for(boost::asio::ip::tcp::resolver::iterator Iterator = resolver.resolve(query);
            Iterator != boost::asio::ip::tcp::resolver::iterator();
            Iterator++)
        {
            boost::asio::ip::tcp::endpoint EndPt = *Iterator;
            //IPAddrs.push_back(EndPt.address().to_string());
        }
        IPAddrsResolved = true;
    }
    catch(boost::wrapexcept<boost::system::system_error> error)
    {
        std::cout << error.what() << std::endl;
    }
    
    return IPAddrsResolved;
}


