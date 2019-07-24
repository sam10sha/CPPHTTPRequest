#include "stdafx.h"
#include "WebClient.h"

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
    ReceiveResponse(Socket, Response);
    Response = ParseResponse(Response);
    return Response;
}




// Private member functions
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
