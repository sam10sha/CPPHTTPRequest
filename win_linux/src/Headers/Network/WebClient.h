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
	/// <summary>
	/// Utiliy for sending web requests.
	/// </summary>
    class WebClient
    {
    // Private enumerators
    private:
    	/// <summary>
    	/// Enumerator for selecting the protocol for the web request.
    	/// </summary>
    	enum RequestProtocol
		{
        	/// <summary>
        	/// HTTP protocol.
        	/// </summary>
    		HTTP,
	    	/// <summary>
	    	/// HTTPS protocol.
	    	/// </summary>
			HTTPS
		};

    // Private member variables
    private:
    	/// <summary>
    	/// The delimiter indicating the end of a line in a web request.
    	/// </summary>
        const std::string RequestDelimiter;
    	/// <summary>
    	/// The protocol to be used for the request.
    	/// </summary>
        RequestProtocol Protocol;
    	/// <summary>
    	/// A reference to an instance of IO service to be used for the socket operations.
    	/// </summary>
        boost::asio::io_service* IOService;
    	/// <summary>
    	/// A pointer to an instance of SSL context to be used during HTTPS requests.
    	/// </summary>
        boost::asio::ssl::context* SSLContext;
    	/// <summary>
    	/// A pointer to an instance of an HTTP socket.
    	/// </summary>
        boost::asio::ip::tcp::socket* HTTPSocket;
    	/// <summary>
    	/// A pointer to an instance of an HTTPS socket stream.
    	/// </summary>
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket>* HTTPSSocket;
    	/// <summary>
    	/// A callback structure defining the method for retrieving additional data for the body content
    	/// stream.
    	/// </summary>
        GenericIO::GenericIStreamBuf::DataRetrieveCallback_member<WebClient> DataRetCB;

    // Public construction/destruction
    public:
    	/// <summary>
    	/// Default constructor for the WebClient class.
    	/// Initializes default values for web request resources.
    	/// </summary>
        WebClient();
    	/// <summary>
    	/// Destructor for the WebClient class.
    	/// Will close any open sockets and deallocate any memory utilized during the request.
    	/// </summary>
        ~WebClient();
        
    // Public member functions
    public:
    	/// <summary>
    	/// Will send the request using the properties specified in 'RequestMsg' and store the
    	/// properties of the response in 'ResponseMsg'.
    	/// </summary>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties are used to send the
    	/// request.
    	/// </param>
    	/// <param name="ResponseMsg">
    	/// A reference to an instance of HttpResponseMessage whose properties are set from the result
    	/// of sending the web request.
    	/// </param>
        void SendRequest(HttpRequestMessage& RequestMsg,
                            HttpResponseMessage& ResponseMsg);
        
    // Private member functions
    private:
    	/// <summary>
    	/// Will send an HTTP request.
    	/// </summary>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties are used to send the
    	/// request.
    	/// </param>
    	/// <param name="ResponseMsg">
    	/// A reference to an instance of HttpResponseMessage whose properties are set from the result
    	/// of sending the web request.
    	/// </param>
        void SendHTTPRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg);
    	/// <summary>
    	/// Will send all headers and content to the remote server.
    	/// </summary>
    	/// <param name="Socket">
    	/// The socket through which the headers and content will be sent.
    	/// </param>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage from which the headers and body content
    	/// will be retrieved.
    	/// </param>
        void MakeRequest(boost::asio::ip::tcp::socket& Socket, const HttpRequestMessage& RequestMsg) const;
    	/// <summary>
    	/// Will receive headers and content from the remote server.
    	/// </summary>
    	/// <param name="Socket">
    	/// The socket through which the response will be received from the remote server.
    	/// </param>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties will be used for the web request.
    	/// </param>
    	/// <param name="ResponseMsg">
    	/// A reference to an instance of HttpResponseMessage into which the headers and content of the response
    	/// will be stored.
    	/// </param>
        void ReceiveResponse(boost::asio::ip::tcp::socket& Socket,
                                const HttpRequestMessage& RequestMsg,
                                HttpResponseMessage& ResponseMsg);
        

    	/// <summary>
    	/// Will send an HTTPS request.
    	/// </summary>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties are used to send the
    	/// request.
    	/// </param>
    	/// <param name="ResponseMsg">
    	/// A reference to an instance of HttpResponseMessage whose properties are set from the result
    	/// of sending the web request.
    	/// </param>
        void SendHTTPSRequest(HttpRequestMessage& RequestMsg, HttpResponseMessage& ResponseMsg);
    	/// <summary>
    	/// Will perform the initial handshake procedure with the remote HTTPS server.
    	/// </summary>
    	/// <param name="RequestMsg"></param>
    	/// <param name="SSLSocket">
    	/// The socket through which the communication will be performed.
    	/// </param>
    	/// <returns>
    	/// A flag indicating the status of the operation.
    	/// True upon success.
    	/// False upon failure.
    	/// </returns>
        bool SSLHandshake(const HttpRequestMessage& RequestMsg, boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket) const;
    	/// <summary>
    	/// Will send all headers and content to the remote server.
    	/// </summary>
    	/// <param name="SSLSocket">
    	/// The socket through which the headers and content will be sent.
    	/// </param>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage from which the headers and body content
    	/// will be retrieved.
    	/// </param>
        void MakeSSLRequest(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket, const HttpRequestMessage& RequestMsg) const;
    	/// <summary>
    	/// Will receive headers and content from the remote server.
    	/// </summary>
    	/// <param name="SSLSocket">
    	/// The socket through which the response will be received from the remote server.
    	/// </param>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties will be used for the web request.
    	/// </param>
    	/// <param name="ResponseMsg">
    	/// A reference to an instance of HttpResponseMessage into which the headers and content of the response
    	/// will be stored.
    	/// </param>
        void ReceiveSSLResponse(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& SSLSocket,
                                        const Network::HttpRequestMessage& RequestMsg,
                                        Network::HttpResponseMessage& ResponseMsg);
        
    // Private general helper functions
    private:
    	/// <summary>
    	/// Will compile a list of IP end points for the remote server.
    	/// </summary>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties are to be used to create
    	/// the list of IP end points.
    	/// </param>
    	/// <param name="IOService">
    	/// A reference to an instance of IO service to be used for the socket operations.
    	/// </param>
    	/// <returns>
    	/// A flag indicating the status of the operation.
    	/// True in the case that IP addresses are successfully resolved.
    	/// False otherwise.
    	/// </returns>
        bool ResolveIPAddrs(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService) const;
    	/// <summary>
    	/// Will attempt to conenct to the remote server.
    	/// </summary>
    	/// <param name="RequestMsg">
    	/// A reference to an instance of HttpRequestMessage whose properties will be used to determine
    	/// the remote end point.
    	/// </param>
    	/// <param name="IOService">
    	/// A reference to an instance of IO service to be used for the socket operations.
    	/// </param>
    	/// <param name="Socket">
    	/// A reference to a socket to be used in order to connect to the remote end point.
    	/// </param>
    	/// <returns>
    	/// A flag indicating the status of the operation.
    	/// True in the case of success.
    	/// False in the case of failure.
    	/// </returns>
        bool ConnectToRemoteServer(HttpRequestMessage& RequestMsg, boost::asio::io_service& IOService, boost::asio::ip::tcp::socket& Socket) const;
    	/// <summary>
    	/// Retrieves the location of the first occurrence of 'StrToFind' in the 'RawData'.
    	/// </summary>
    	/// <param name="RawData">
    	/// The data to search for occurrences of 'StrToFind'.
    	/// </param>
    	/// <param name="RawDataLen">
    	/// The size of 'RawData'.
    	/// </param>
    	/// <param name="StrToFind">
    	/// The string whose location in 'RawData' is to be found.
    	/// </param>
    	/// <returns>
    	/// The location of the first occurrence of 'StrToFind' in 'RawData'.
    	/// -1, if no occurrence of 'StrToFind' is found.
    	/// </returns>
        int FindStr(const void* const RawData, const size_t RawDataLen, const std::string& StrToFind) const;

        // Callbacks
    	/// <summary>
    	/// A callback function defining the method for retrieving additional data for the content body of the
    	/// web response.
    	/// </summary>
    	/// <param name="Buffer">
    	/// A location into which to store the incoming bytes.
    	/// </param>
    	/// <param name="MaxLen">
    	/// The maximum amount of data that can be written.
    	/// </param>
    	/// <returns>
    	/// The number of bytes written into the buffer.
    	/// </returns>
        size_t RetrieveData(void* const Buffer, const size_t MaxLen);
    };
}

#endif // WEBCLIENT_H

