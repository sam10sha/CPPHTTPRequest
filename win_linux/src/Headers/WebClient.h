#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include "HttpRequestMessage.h"
#include "HttpResponseMessage.h"
#include "IStreamWrap.h"

namespace Network
{
    class WebClient
    {
    private:
        std::string RequestDelimiter;
        
    // Public construction
    public:
        WebClient();
        
    // Public member functions
    public:
        void SendRequest(HttpRequestMessage& RequestMsg,
                            HttpResponseMessage& ResponseMsg) const;
        
    // Private member functions
    private:
        void SendHTTPRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg) const;
        void MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveResponse(boost::asio::ip::tcp::socket& Socket,
                                const HttpRequestMessage& RequestMsg,
                                HttpResponseMessage& ResponseMsg) const;
        
        
        void SendHTTPSRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg) const;
        void MakeSSLRequest(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveSSLResponse(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket,
                                const Network::HttpRequestMessage& RequestMsg,
                                Network::HttpResponseMessage& ResponseMsg) const;
        
    // Private general helper functions
    private:
        bool ResolveIPAddrs(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService) const;
        bool ConnectToRemoteServer(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService, boost::asio::ip::tcp::socket& Socket) const;
        bool SSLHandshake(const HttpRequestMessage& RequestMsg, boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket) const;
        int FindStr(const void* const RawData, const size_t RawDataLen, const std::string& StrToFind) const;
    };
}

#endif // WEBCLIENT_H

