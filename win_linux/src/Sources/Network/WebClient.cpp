#include "stdafx.h"
#include "HttpStreamContent.h"
#include "HttpRequestMessage.h"
#include "HttpResponseMessage.h"
#include "WebClient.h"

// Public construction
Network::WebClient::WebClient() :
	RequestDelimiter("\r\n"),
	Protocol(HTTP),
	IOService(nullptr),
	SSLContext(nullptr),
	HTTPSocket(nullptr),
	HTTPSSocket(nullptr),
	DataRetCB(this, &WebClient::RetrieveData)
{

}
Network::WebClient::~WebClient()
{
	if(HTTPSocket)
	{
		try
		{
			HTTPSocket->close();
		}
		catch(boost::wrapexcept<boost::system::system_error>& e)
		{

		}
		delete HTTPSocket;
		HTTPSocket = nullptr;
	}
	if(HTTPSSocket)
	{
		try
		{
			HTTPSSocket->shutdown();
			((boost::asio::ip::tcp::socket&)HTTPSSocket->lowest_layer()).close();
		}
		catch(boost::wrapexcept<boost::system::system_error>& e)
		{

		}
		delete HTTPSSocket;
		HTTPSSocket = nullptr;
	}
	if(IOService)
	{
		delete IOService;
		IOService = nullptr;
	}
	if(SSLContext)
	{
		delete SSLContext;
		SSLContext = nullptr;
	}
}


// Public member functions
void Network::WebClient::SendRequest(Network::HttpRequestMessage& RequestMsg,
                                        Network::HttpResponseMessage& ResponseMsg)
{
    std::string Protocol = RequestMsg.GetProtocol();
    if(Protocol == "http")
    {
        SendHTTPRequest(RequestMsg, ResponseMsg);
    }
    else if(Protocol == "https")
    {
        SendHTTPSRequest(RequestMsg, ResponseMsg);
    }

    ResponseMsg.SetMethod(RequestMsg.GetMethod());
	ResponseMsg.SetURL(RequestMsg.GetURL());
	ResponseMsg.SetServerHostName(RequestMsg.GetServerHostName());
	ResponseMsg.SetServerIPAddr(RequestMsg.GetServerIPAddr());
	ResponseMsg.SetPort(RequestMsg.GetPort());
	ResponseMsg.SetQueryPath(RequestMsg.GetQueryPath());
}




// Private member functions
void Network::WebClient::SendHTTPRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg)
{
    bool ConnectionSuccess = false;
    boost::system::error_code ErrorCode;
    Protocol = HTTP;
	
    IOService = new boost::asio::io_service;
    HTTPSocket = new boost::asio::ip::tcp::socket(*IOService);
    
    ConnectionSuccess = ConnectToRemoteServer(RequestMsg, *IOService, *HTTPSocket);
    if(ConnectionSuccess)
    {
        MakeRequest(*HTTPSocket, RequestMsg);
        ReceiveResponse(*HTTPSocket, RequestMsg, ResponseMsg);
    }
}
void Network::WebClient::MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const
{
	std::istream* ContentStream = nullptr;
    boost::system::error_code Error;
    // Create request
    boost::asio::streambuf Request;
    std::ostream RequestStream(&Request);
	
	
    // Send request header
    RequestStream << RequestMsg.GetRequestHeaderSection();
    RequestStream << RequestDelimiter;
    boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
	
    // Send request content
    ContentStream = RequestMsg.GetRequestBodyStream();
    if(ContentStream)
    {
        RequestStream << ContentStream->rdbuf();
        RequestStream << RequestDelimiter;
        boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
    }
}
void Network::WebClient::ReceiveResponse(boost::asio::ip::tcp::socket& Socket,
                                            const Network::HttpRequestMessage& RequestMsg,
                                            Network::HttpResponseMessage& ResponseMsg)
{
    boost::system::error_code Error;
    // Create response buffer
    boost::asio::streambuf Response;
    const std::string HeadBodyDelimiter("\r\n\r\n");
    int HeadBodyDelimiterLocation = 0;
    std::string Headers;
    HttpStreamContent* StreamContent = nullptr;
    
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
    StreamContent = new HttpStreamContent(0x200, 0);
    StreamContent->RegisterDataCB(&DataRetCB);
    StreamContent->InjectData(
		boost::asio::buffer_cast<const char*>(Response.data()) + HeadBodyDelimiterLocation + HeadBodyDelimiter.size(),
		Response.size() - HeadBodyDelimiterLocation - HeadBodyDelimiter.size()
    );

    // Set response message components
    ResponseMsg.ParseRawHeader(Headers);
    ResponseMsg.SetContent(StreamContent);
}




void Network::WebClient::SendHTTPSRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg)
{
    bool ConnectionSuccess = false;
    bool HandshakeSuccess = false;
    namespace ssl = boost::asio::ssl;
    boost::system::error_code ErrorCode;
    Protocol = HTTPS;
    
    IOService = new boost::asio::io_service;

    // Create a context that uses the default paths for finding CA certificates.
    SSLContext = new ssl::context(ssl::context::sslv23);
    SSLContext->set_default_verify_paths();
    
    HTTPSSocket = new ssl::stream<boost::asio::ip::tcp::socket>(*IOService, *SSLContext);
    ConnectionSuccess = ConnectToRemoteServer(RequestMsg, *IOService, (boost::asio::ip::tcp::socket&)HTTPSSocket->lowest_layer());
    if(ConnectionSuccess)
    {
        //SSLSocket.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
        HandshakeSuccess = SSLHandshake(RequestMsg, *HTTPSSocket);
    }
    if(HandshakeSuccess)
    {
        MakeSSLRequest(*HTTPSSocket, RequestMsg);
        ReceiveSSLResponse(*HTTPSSocket, RequestMsg, ResponseMsg);
    }
}
bool Network::WebClient::SSLHandshake(const HttpRequestMessage& RequestMsg, boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket) const
{
    bool HandshakeSuccess = false;
    namespace ssl = boost::asio::ssl;
    
    // Perform SSL handshake and verify the remote host's certificate.
    SSLSocket.set_verify_mode(ssl::verify_peer);
    //SSLSocket.set_verify_mode(ssl::verify_none);
    SSLSocket.set_verify_callback(ssl::rfc2818_verification(RequestMsg.GetServerHostName()));
    SSL_set_tlsext_host_name(SSLSocket.native_handle(), RequestMsg.GetServerHostName().c_str());
    try
    {
        SSLSocket.handshake(ssl::stream<boost::asio::ip::tcp::socket>::client);
        HandshakeSuccess = true;
    }
    catch(std::exception& e)
    {
        std::cout << "WebClient[SSLHandshake]: " << e.what() << std::endl;
    }
    return HandshakeSuccess;
}
void Network::WebClient::MakeSSLRequest(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket, const HttpRequestMessage& RequestMsg) const
{
	std::istream* ContentStream = nullptr;
    boost::system::error_code Error;
    // Create request
    boost::asio::streambuf Request;
    std::ostream RequestStream(&Request);
        
    // Send request header
    RequestStream << RequestMsg.GetRequestHeaderSection();
    RequestStream << RequestDelimiter;
    boost::asio::write(SSLSocket, Request, boost::asio::transfer_all(), Error);
	
    // Send request content
    ContentStream = RequestMsg.GetRequestBodyStream();
    if(ContentStream)
    {
        RequestStream << ContentStream->rdbuf();
        RequestStream << RequestDelimiter;
        boost::asio::write(SSLSocket, Request, boost::asio::transfer_all(), Error);
    }
}
void Network::WebClient::ReceiveSSLResponse(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket,
                                            const Network::HttpRequestMessage& RequestMsg,
                                            Network::HttpResponseMessage& ResponseMsg)
{
    boost::system::error_code Error;
    // Create response buffer
    boost::asio::streambuf Response;
    const std::string HeadBodyDelimiter("\r\n\r\n");
    int HeadBodyDelimiterLocation = 0;
    std::string Headers;
    HttpStreamContent* StreamContent = nullptr;

    // Receive headers
    boost::asio::read_until(SSLSocket, Response, HeadBodyDelimiter.c_str(), Error);
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
    StreamContent = new HttpStreamContent(0x200, 0);
    //StreamContent = new HttpStreamContent(0x10000, 0);
    StreamContent->RegisterDataCB(&DataRetCB);
    StreamContent->InjectData(
		boost::asio::buffer_cast<const char*>(Response.data()) + HeadBodyDelimiterLocation + HeadBodyDelimiter.size(),
		Response.size() - HeadBodyDelimiterLocation - HeadBodyDelimiter.size()
    );

    // Set response message components
    ResponseMsg.ParseRawHeader(Headers);
    ResponseMsg.SetContent(StreamContent);
}




// Private general helper functions
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
bool Network::WebClient::ConnectToRemoteServer(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService, boost::asio::ip::tcp::socket& Socket) const
{
    bool IPAddrsResolved = false;
    bool ConnectionSuccess = false;
    IPAddrsResolved = ResolveIPAddrs(RequestMsg, IOService);
    if(IPAddrsResolved)
    {
        const std::vector<HttpRequestMessage::IPEndPoint>& RemoteServerIPEndPts = RequestMsg.GetRemoteServerIPEndPts();
        std::vector<boost::asio::ip::tcp::endpoint> BoostEndPts;
        for(std::vector<HttpRequestMessage::IPEndPoint>::const_iterator IPEndPt = RemoteServerIPEndPts.begin();
            !ConnectionSuccess && IPEndPt != RemoteServerIPEndPts.end();
            IPEndPt++)
        {
            boost::asio::ip::tcp::endpoint EndPoint(
                boost::asio::ip::address::from_string(IPEndPt->mIPAddr.c_str()),
                (int)IPEndPt->mPort
            );
            BoostEndPts.push_back(EndPoint);
        }
        try
        {
            boost::asio::connect(Socket, BoostEndPts.begin());
            RequestMsg.SetRemoteServerIPAddr(Socket.remote_endpoint().address().to_string());
            RequestMsg.SetRemoteServerPort(Socket.remote_endpoint().port());
            ConnectionSuccess = true;
        }
        catch(std::exception& e)
        {
            std::cout << "WebClient[ConnectToRemoteServer]: " << e.what() << std::endl;
        }
    }
    return ConnectionSuccess;
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


// Private callbacks
size_t Network::WebClient::RetrieveData(void* const Buffer, const size_t MaxLen)
{
	size_t NumBytesRead = 0;
    boost::system::error_code Error;
    switch(Protocol)
    {
		case HTTP:
			NumBytesRead = boost::asio::read(*HTTPSocket, boost::asio::buffer(Buffer, MaxLen), Error);
			break;
		case HTTPS:
			NumBytesRead = HTTPSSocket->read_some(boost::asio::buffer(Buffer, MaxLen), Error);
			//NumBytesRead = boost::asio::read(*HTTPSSocket, boost::asio::buffer(Buffer, MaxLen), Error);
			break;
    }
	return NumBytesRead;
}
