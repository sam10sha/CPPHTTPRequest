#include "stdafx.h"
#include "WebClient.h"

// Public construction
Network::WebClient::WebClient()
	: RequestDelimiter("\r\n") { }


// Public member functions
std::string Network::WebClient::SendRequest(const Network::HttpRequestMessage& RequestMsg) const
{
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::socket Socket(IOService);
    boost::asio::ip::tcp::endpoint EndPoint(
        boost::asio::ip::address::from_string(RequestMsg.GetServerIPAddress().c_str()),
        (int)RequestMsg.GetPort()
    );
    std::string Response;
    
    Socket.connect(EndPoint);
    MakeRequest(Socket, RequestMsg);
    ReceiveResponse_2(Socket, Response);
    Response = ParseResponse(Response);
	
    return Response;
}

void Network::WebClient::SendRequest_2(const Network::HttpRequestMessage& RequestMsg,
											Network::HttpResponseMessage& ResponseMsg) const
{
    boost::asio::io_service IOService;
    boost::asio::ip::tcp::socket Socket(IOService);
    boost::asio::ip::tcp::endpoint EndPoint(
        boost::asio::ip::address::from_string(RequestMsg.GetServerIPAddress().c_str()),
        (int)RequestMsg.GetPort()
    );
    
	std::cout << "WebClient[SendRequest_2]: Connecting..." << std::endl;
    Socket.connect(EndPoint);
	std::cout << "WebClient[SendRequest_2]: Sending request..." << std::endl;
    MakeRequest(Socket, RequestMsg);
	std::cout << "WebClient[SendRequest_2]: Receiving response..." << std::endl;
    ReceiveResponse_3(Socket, RequestMsg, ResponseMsg);
	std::cout << "WebClient[SendRequest_2]: Done" << std::endl;
	std::cout << "WebClient[SendRequest_2]: Response status code: " << ResponseMsg.GetResponseStatusCode() << std::endl;
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
    RequestStream << RequestMsg.GetAllRequestHeaders();
	RequestStream << RequestDelimiter;
    boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
	
	// Send request content
	RequestMsg.GetRequestBodyStream(StreamWrap);
	std::istream& Stream = *StreamWrap.mStream;
	RequestStream << Stream.rdbuf();
	RequestStream << RequestDelimiter;
    boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
}
void Network::WebClient::MakeRequest_2(boost::asio::ip::tcp::socket& Socket, const Network::HttpRequestMessage& RequestMsg) const
{
    boost::system::error_code Error;
    // Create request
    boost::asio::streambuf Request;
    std::ostream RequestStream(&Request);
    RequestStream << RequestMsg.GetRequest();
    
    // Send request
    boost::asio::write(Socket, Request, boost::asio::transfer_all(), Error);
}
void Network::WebClient::ReceiveResponse(boost::asio::ip::tcp::socket& Socket, HttpResponseMessage& ResponseMsg) const
{
	boost::system::error_code Error;
    // Create response buffer
    boost::asio::streambuf Response;
    //boost::asio::read_until(Socket, Response, Error);
}
void Network::WebClient::ReceiveResponse_2(boost::asio::ip::tcp::socket& Socket, std::string& ReceivedResponse) const
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
void Network::WebClient::ReceiveResponse_3(boost::asio::ip::tcp::socket& Socket,
																	const Network::HttpRequestMessage& RequestMsg,
																	Network::HttpResponseMessage& ResponseMsg) const
{
	boost::system::error_code Error;
    // Create response buffer
    boost::asio::streambuf Response;
	std::string Headers;
	std::string Content;
	
	boost::asio::read_until(Socket, Response, "\r\n\r\n", Error);
	Headers = boost::asio::buffer_cast<const char*>(Response.data());
	boost::asio::read(Socket, Response, Error);
	Content = boost::asio::buffer_cast<const char*>(Response.data());
	
	std::cout << "WebClient[ReceiveResponse_3]: Headers: " << Headers << std::endl;
	std::cout << "WebClient[ReceiveResponse_3]: Content: " << Content << std::endl;
	
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
// UNSAFE! CAUTION FOR MEMORY LEAK!
void Network::WebClient::TransferBytesDynamically(std::istream& Stream, char** const StoragePtr) const
{
	const size_t INCREMENT_AMT = 1 << 12; // 4 KB
	const size_t BUF_STORAGE_INCREMENT_AMT = 10;	// 10 storage pointers
	char** BufferStoragePtrs = new char*[BUF_STORAGE_INCREMENT_AMT];
	int BufStoragePtrIndex = 0;
	
	BufferStoragePtrs[BufStoragePtrIndex] = new char[INCREMENT_AMT];
	
	*StoragePtr = new char[INCREMENT_AMT];
}
