#ifndef WEBCLIENT_H
#define WEBCLIENT_H

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
        bool ConnectToRemoteServer(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService, boost::asio::ip::tcp::socket& Socket) const;
        bool ResolveIPAddrs(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService) const;
        void MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveResponse(boost::asio::ip::tcp::socket& Socket,
                                const HttpRequestMessage& RequestMsg,
                                HttpResponseMessage& ResponseMsg) const;
        int FindStr(const void* const RawData, const size_t RawDataLen, const std::string& StrToFind) const;
    };
}

#endif // WEBCLIENT_H
