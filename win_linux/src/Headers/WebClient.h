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
        std::string SendRequest(const HttpRequestMessage& RequestMsg) const;
        void SendRequest_2(const HttpRequestMessage& RequestMsg,
											HttpResponseMessage& ResponseMsg) const;
        
	// Private member functions
    private:
		void MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
        void MakeRequest_2(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
        void ReceiveResponse(boost::asio::ip::tcp::socket& Socket, HttpResponseMessage& ResponseMsg) const;
        void ReceiveResponse_2(boost::asio::ip::tcp::socket& Socket, std::string& ReceivedResponse) const;
        void ReceiveResponse_3(boost::asio::ip::tcp::socket& Socket,
								const HttpRequestMessage& RequestMsg,
								HttpResponseMessage& ResponseMsg) const;
		std::string ParseResponse(const std::string& ServerResponse) const;
		// UNSAFE! CAUTION FOR MEMORY LEAK!
		void TransferBytesDynamically(std::istream& Stream, char** const StoragePtr) const;
    };
}

#endif // WEBCLIENT_H
