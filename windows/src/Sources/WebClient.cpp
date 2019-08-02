#include "stdafx.h"
#include "WebClient.h"

// Public member functions
std::string Network::WebClient::SendRequest(const Network::HttpRequestMessage& RequestMsg) const
{
#ifdef OS_LINUX
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::socket Socket(IOService);
    boost::asio::ip::tcp::endpoint EndPoint(
        boost::asio::ip::address::from_string(RequestMsg.GetServerIPAddress().c_str()),
        (int)RequestMsg.GetPort()
    );
    std::string Response;
    
    Socket.connect(EndPoint);
    MakeRequest(Socket, RequestMsg);
    ReceiveResponse(Socket, Response);
    Response = ParseResponse(Response);
    return Response;
#elif defined(OS_WIN)
	bool ServerConnected = false;
	bool RequestSent = false;
	bool ResponseReceived = false;
	SOCKET Socket = socket(AF_INET, SOCK_STREAM, 0);
	std::string Response;
	
	ServerConnected = ConnectToServer(Socket, RequestMsg);
	if(ServerConnected)
	{
		RequestSent = MakeRequest(Socket, RequestMsg);
	}
	if(RequestSent == true)
	{
		ResponseReceived = ReceiveResponse(Socket, Response);
	}
	if(ResponseReceived == true)
	{
		Response = ParseResponse(Response);
	}
	return Response;
#else // OS_VERSION
	return std::string();
#endif // OS_VERSION
}




// Private member functions
#ifdef OS_LINUX
void Network::WebClient::MakeRequest(boost::asio::ip::tcp::socket& Socket, const Network::HttpRequestMessage& RequestMsg) const
{
    boost::system::error_code Error;
    // Create request
    boost::asio::streambuf Request;
    std::ostream RequestStream(&Request);
    RequestStream << RequestMsg.GetRequest();
    
    // Send request
    boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
}
void Network::WebClient::ReceiveResponse(boost::asio::ip::tcp::socket& Socket, std::string& ReceivedResponse) const
{
    boost::system::error_code Error;
    // Create response buffer
    boost::asio::streambuf Response;
    boost::asio::read(Socket, Response, boost::asio::transfer_all(), Error);
    
    if(Error && Error != boost::asio::error::eof)
    {
        ReceivedResponse += "receive failed: " + Error.message() + "\n";
    }
    else
    {
        const char* Data = boost::asio::buffer_cast<const char*>(Response.data());
        ReceivedResponse += Data;
    }
}
#elif (defined(OS_WIN))
bool Network::WebClient::ConnectToServer(const SOCKET Socket, const Network::HttpRequestMessage& RequestMsg) const
{
	int ConnectError = 0;
	struct sockaddr_in SockAddress;
	SockAddress.sin_family = AF_INET;
	SockAddress.sin_addr.s_addr = inet_addr(RequestMsg.GetServerIPAddress().c_str());
	SockAddress.sin_port = htons((unsigned short)RequestMsg.GetPort());
	ConnectError = connect(Socket, (const sockaddr *)&SockAddress, sizeof(SockAddress));
	return ConnectError == 0;
}
bool Network::WebClient::MakeRequest(const SOCKET Socket, const Network::HttpRequestMessage& RequestMsg) const
{
	int NumBytesWritten = 0;
	std::string Request = RequestMsg.GetRequest();
	NumBytesWritten = send(Socket, Request.c_str(), (int)Request.length(), 0);
	return NumBytesWritten == Request.length();
}
bool Network::WebClient::ReceiveResponse(const SOCKET Socket, std::string& ReceivedResponse) const
{
	int NumBytesReceived = 0;
	const size_t BufferLength = 1 << 12; // 4 KB
	char Buffer[BufferLength];
	ReceivedResponse.clear();
	do
	{
		std::memset(Buffer, 0, BufferLength);
		NumBytesReceived = recv(Socket, Buffer, BufferLength - 1, 0);
		if(NumBytesReceived > 0)
		{
			ReceivedResponse += Buffer;
		}
	} while(NumBytesReceived > 0);
	return ReceivedResponse.length() > 0;
}
#endif // OS_VERSION

std::string Network::WebClient::ParseResponse(const std::string& ServerResponse) const
{
    std::string Body;
    size_t BodyStartLocation = ServerResponse.find("\r\n\r\n");
    if(BodyStartLocation != std::string::npos && BodyStartLocation < ServerResponse.length() - 4)
    {
        Body = ServerResponse.substr(BodyStartLocation + 4);
    }
    return Body;
}
