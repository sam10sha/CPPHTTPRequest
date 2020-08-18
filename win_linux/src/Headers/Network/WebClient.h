#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>

#include "HttpRequestMessage.h"
#include "HttpResponseMessage.h"
#include "GenericIStreamBuf.h"

namespace Network
{
    class WebClient
    {
    // Private enumerators
    private:
    	enum RequestProtocol
		{
    		HTTP,
			HTTPS
		};

    // Private member variables
    private:
        const std::string RequestDelimiter;
        RequestProtocol Protocol;
        boost::asio::io_service* IOService;
        boost::asio::ssl::context* SSLContext;
        boost::asio::ip::tcp::socket* HTTPSocket;
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* HTTPSSocket;
        GenericIO::GenericIStreamBuf::DataRetrieveCallback_member<WebClient> DataRetCB;

    // Public construction/destruction
    public:
        WebClient();
        ~WebClient();
        
    // Public member functions
    public:
        void SendRequest(HttpRequestMessage& RequestMsg,
                            HttpResponseMessage& ResponseMsg);
        
    // Private member functions
    private:
        void SendHTTPRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg);
        void MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveResponse(boost::asio::ip::tcp::socket& Socket,
                                const HttpRequestMessage& RequestMsg,
                                HttpResponseMessage& ResponseMsg);
        
        
        void SendHTTPSRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg);
        bool SSLHandshake(const HttpRequestMessage& RequestMsg, boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket) const;
        void MakeSSLRequest(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveSSLResponse(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket,
                                        const Network::HttpRequestMessage& RequestMsg,
                                        Network::HttpResponseMessage& ResponseMsg);
        
    // Private general helper functions
    private:
        bool ResolveIPAddrs(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService) const;
        bool ConnectToRemoteServer(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService, boost::asio::ip::tcp::socket& Socket) const;
        int FindStr(const void* const RawData, const size_t RawDataLen, const std::string& StrToFind) const;

        // Callbacks
        size_t RetrieveData(void* const Buffer, const size_t MaxLen);
    };
}

#endif // WEBCLIENT_H

