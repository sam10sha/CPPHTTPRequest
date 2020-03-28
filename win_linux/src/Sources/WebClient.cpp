#include "stdafx.h"
#include "WebClient.h"

// Public construction
Network::WebClient::WebClient()
    : RequestDelimiter("\r\n") { }


// Public member functions
void Network::WebClient::SendRequest(Network::HttpRequestMessage& RequestMsg,
                                        Network::HttpResponseMessage& ResponseMsg) const
{
    bool ConnectionSuccess = false;
    boost::system::error_code ErrorCode;
	
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::socket Socket(IOService);
    
    ConnectionSuccess = ConnectToRemoteServer(RequestMsg, IOService, Socket);
    if(ConnectionSuccess)
    {
        MakeRequest(Socket, RequestMsg);
        ReceiveResponse(Socket, RequestMsg, ResponseMsg);
        
        Socket.close(ErrorCode);
    }
}




// Private member functions
bool Network::WebClient::ConnectToRemoteServer(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService, boost::asio::ip::tcp::socket& Socket) const
{
    bool IPAddrsResolved = false;
    bool ConnectionSuccess = false;
    IPAddrsResolved = ResolveIPAddrs(RequestMsg, IOService);
    if(IPAddrsResolved)
    {
        const std::vector<HttpRequestMessage::IPEndPoint>& RemoteServerIPEndPts = RequestMsg.GetRemoteServerIPEndPts();
        for(std::vector<HttpRequestMessage::IPEndPoint>::const_iterator IPEndPt = RemoteServerIPEndPts.begin();
            !ConnectionSuccess && IPEndPt != RemoteServerIPEndPts.end();
            IPEndPt++)
        {
            boost::asio::ip::tcp::endpoint EndPoint(
                boost::asio::ip::address::from_string(IPEndPt->mIPAddr.c_str()),
                (int)IPEndPt->mPort
            );
            try
            {
                Socket.connect(EndPoint);
                RequestMsg.SetRemoteServerIPAddr(IPEndPt->mIPAddr);
                RequestMsg.SetRemoteServerPort(IPEndPt->mPort);
                ConnectionSuccess = true;
            }
            catch(boost::system::system_error error) { }
        }
    }
    return ConnectionSuccess;
}
bool Network::WebClient::ResolveIPAddrs(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService) const
{
    bool IPAddrsResolved = false;
    long RequestedPort = RequestMsg.GetPort();
    boost::asio::ip::tcp::resolver Resolver(IOService);
    boost::asio::ip::tcp::resolver::query Query(RequestMsg.GetServerHostName(), RequestMsg.GetProtocol());
    try
    {
        for(boost::asio::ip::tcp::resolver::iterator Iterator = Resolver.resolve(Query);
            Iterator != boost::asio::ip::tcp::resolver::iterator();
            Iterator++)
        {
            boost::asio::ip::tcp::endpoint EndPt = *Iterator;
            RequestMsg.AddRemoteServerEndPt(
                HttpRequestMessage::IPEndPoint(
                    EndPt.address().to_string(),
                    RequestedPort < 0 ? EndPt.port() : RequestedPort
                )
            );
        }
        IPAddrsResolved = true;
    }
    catch(int error) { }
    return IPAddrsResolved;
}
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
    ResponseMsg.SetServerIPAddr(RequestMsg.GetServerIPAddr());
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


