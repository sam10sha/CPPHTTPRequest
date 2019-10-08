#include "stdafx.h"
#include "WebClient.h"

// Public construction
Network::WebClient::WebClient()
    : RequestDelimiter("\r\n") { }


// Public member functions
void Network::WebClient::SendRequest(const Network::HttpRequestMessage& RequestMsg,
                                        Network::HttpResponseMessage& ResponseMsg) const
{
	boost::system::error_code ErrorCode;
	
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::socket Socket(IOService);
    boost::asio::ip::tcp::endpoint EndPoint(
        boost::asio::ip::address::from_string(RequestMsg.GetServerIPAddress().c_str()),
        (int)RequestMsg.GetPort()
    );
    
    Socket.connect(EndPoint);
    MakeRequest(Socket, RequestMsg);
    ReceiveResponse(Socket, RequestMsg, ResponseMsg);
	
	Socket.close(ErrorCode);
}




// Private member functions
void Network::WebClient::MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const
{
    IStreamWrap StreamWrap;
    boost::system::error_code Error;
    // Create request
    boost::asio::streambuf Request;
    std::ostream RequestStream(&Request);
	
	
    // Send request header
    RequestStream << RequestMsg.GetRequestHeaderSection();
    RequestStream << RequestDelimiter;
    boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
	
    // Send request content
    RequestMsg.GetRequestBodyStream(StreamWrap);
	if(StreamWrap.mStream)
	{
		std::istream& Stream = *StreamWrap.mStream;
		RequestStream << Stream.rdbuf();
		RequestStream << RequestDelimiter;
		boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
	}
}
void Network::WebClient::ReceiveResponse(boost::asio::ip::tcp::socket& Socket,
                                            const Network::HttpRequestMessage& RequestMsg,
                                            Network::HttpResponseMessage& ResponseMsg) const
{
    boost::system::error_code Error;
    // Create response buffer
    boost::asio::streambuf Response;
    const std::string HeadBodyDelimiter("\r\n\r\n");
    int HeadBodyDelimiterLocation = 0;
    std::string Headers;
    std::string Content;
    
    // Receive headers
    boost::asio::read_until(Socket, Response, HeadBodyDelimiter.c_str(), Error);
    HeadBodyDelimiterLocation = FindStr(boost::asio::buffer_cast<const char*>(Response.data()), Response.size(), HeadBodyDelimiter);
    if(HeadBodyDelimiterLocation >= 0)
    {
        Headers = std::string(boost::asio::buffer_cast<const char*>(Response.data()), HeadBodyDelimiterLocation);
    }
    else    // if no delimiter found, assume entire response is header
    {
        Headers = boost::asio::buffer_cast<const char*>(Response.data());
    }
    // Receive content
    boost::asio::read(Socket, Response, Error);
    if(HeadBodyDelimiterLocation >= 0                                               // if no delimiter found, assume entire response is header
        && HeadBodyDelimiterLocation + HeadBodyDelimiter.size() < Response.size())  // if delimiter is at the end of the response, leave content blank
    {
        Content = std::string(
            boost::asio::buffer_cast<const char*>(Response.data()) + HeadBodyDelimiterLocation + HeadBodyDelimiter.size(),
            Response.size() - HeadBodyDelimiterLocation - HeadBodyDelimiter.size()
        );
    }
    
    // Set response message components
    ResponseMsg.SetMethod(RequestMsg.GetMethod());
    ResponseMsg.SetURL(RequestMsg.GetURL());
    ResponseMsg.SetServerHostName(RequestMsg.GetServerHostName());
    ResponseMsg.SetServerIPAddr(RequestMsg.GetServerIPAddress());
    ResponseMsg.SetPort(RequestMsg.GetPort());
    ResponseMsg.SetQueryPath(RequestMsg.GetQueryPath());
    ResponseMsg.ParseRawHeader(Headers);
    ResponseMsg.SetStringContent(HttpStringContent(Content));
}
int Network::WebClient::FindStr(const void* const RawData, const size_t RawDataLen, const std::string& StrToFind) const
{
    int Location = -1;
    if(StrToFind.empty() == false)
    {
        const size_t StrToFindLen = StrToFind.size();
        for(int i = 0; i <= (int)(RawDataLen - StrToFindLen) && Location < 0; i++)
        {
            if(!std::memcmp(((const char*)RawData) + i, StrToFind.c_str(), StrToFindLen))
            {
                Location = i;
            }
        }
    }
    return Location;
}


