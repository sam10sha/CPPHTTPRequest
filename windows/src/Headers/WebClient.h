#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <string>
#include "HttpRequestMessage.h"

namespace Network
{
    class WebClient
    {
    public:
        std::string SendRequest(const HttpRequestMessage& RequestMsg) const;
        
    private:
        void MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveResponse(boost::asio::ip::tcp::socket& Socket, std::string& ReceivedResponse) const;
		/* bool ConnectToServer(const SOCKET Socket, const HttpRequestMessage& RequestMsg) const;
		bool MakeRequest(const SOCKET Socket, const HttpRequestMessage& RequestMsg) const;
		bool ReceiveResponse(const SOCKET Socket, std::string& ReceivedResponse) const; */
        std::string ParseResponse(const std::string& ServerResponse) const;
    };
}

#endif // WEBCLIENT_H
