#ifndef HTTPREQUESTMESSAGE_H
#define HTTPREQUESTMESSAGE_H

#include <map>
#include <string>
#include <vector>

#include "HttpByteContent.h"
#include "HttpContent.h"
#include "HttpFileStreamContent.h"
#include "HttpStringContent.h"

namespace Network
{
	/// <summary>
	/// Component storing information about an HTTP request message.
	/// </summary>
    class HttpRequestMessage
    {
    // Public structures
    public:
    	/// <summary>
    	/// Structure containing information about an IP address end point.
    	/// </summary>
        struct IPEndPoint
        {
        	/// <summary>
        	/// The IP address of the end point.
        	/// </summary>
            std::string mIPAddr;
        	/// <summary>
        	/// The port number of the end point.
        	/// </summary>
            long mPort;

        	/// <summary>
        	/// Constructor for the IP End Point structure.
        	/// </summary>
        	/// <param name="IPAddr">
        	/// IP address to store.
        	/// </param>
        	/// <param name="Port">
        	/// The port number to store.
        	/// </param>
            IPEndPoint(const std::string& IPAddr, const long Port) :
                mIPAddr(IPAddr),
                mPort(Port) { }
        	/// <summary>
        	/// Overloaded equals operator. Will store the properties of the specified
        	/// IP End Point instance to the current instance.
        	/// </summary>
        	/// <param name="EndPt">
        	/// The instance of the IPEndPoint structure to copy.
        	/// </param>
            void operator=(const IPEndPoint& EndPt)
            {
                mIPAddr.clear();
                mIPAddr.append(EndPt.mIPAddr);
                mPort = EndPt.mPort;
            }
        };
        
        
    // Public construction/destruction
    public:
    	/// <summary>
    	/// Default constructor for the HttpRequestMessage class.
    	/// Sets the method, protocol, server name, port and query string to default values.
    	/// </summary>
        HttpRequestMessage();
    	/// <summary>
    	/// Constructor for the HttpRequestMessage class.
    	/// Parses the specified URL for the protocol, server name, port, and query string.
    	/// </summary>
    	/// <param name="Method">
    	/// The method to use to send the request.
    	/// </param>
    	/// <param name="URL">
    	/// The URL of the target.
    	/// </param>
        HttpRequestMessage(const std::string& Method, const std::string& URL);
    	/// <summary>
    	/// Copy constructor for the HttpRequestMessage class.
        /// Deleted function. Copying instances of this class is not permitted.
    	/// </summary>
    	/// <param name="RequestMsg">
		/// A reference to the instance of the HttpRequestMessage class to copy.
		/// </param>
        HttpRequestMessage(const HttpRequestMessage& RequestMsg) = delete;
    	/// <summary>
    	/// Destructor for the HttpRequestMessage class.
		/// Will deallocate the memory used by the HttpContent body, if specified and managed by
		/// the HttpRequestMessage instance.
    	/// </summary>
        ~HttpRequestMessage();
        	
    // Public operator overloads
    public:
    	/// <summary>
    	/// Overloaded equals operator. Will copy properties of specified instance of HttpRequestMessage class.
		/// Deleted function. Copying instances of this class is not permitted.
    	/// </summary>
    	/// <param name="RequestMsg">
    	/// A reference to the instance of HttpRequestMessage to copy.
    	/// </param>
        void operator=(const HttpRequestMessage& RequestMsg) = delete;
        
    // Public methods
    public:
    	/// <summary>
    	/// Retrieves the method for the request.
    	/// </summary>
    	/// <returns>
    	/// The method for the request.
    	/// </returns>
        std::string GetMethod() const;
    	/// <summary>
    	/// Retrieves the URL for the request.
    	/// </summary>
    	/// <returns>
    	/// The URL for the request.
    	/// </returns>
        std::string GetURL() const;
    	/// <summary>
    	/// Retrieves the protocol for the request.
    	/// </summary>
    	/// <returns>
    	/// The protocol for the request.
    	/// </returns>
        std::string GetProtocol() const;
    	/// <summary>
    	/// Retrieves the target server host name.
    	/// </summary>
    	/// <returns>
    	/// The target server host name.
    	/// </returns>
        std::string GetServerHostName() const;
    	/// <summary>
    	/// Retrieves a list of all possible end points for the specified
    	/// target server.
    	/// </summary>
    	/// <returns>
    	/// The list of all possible end points for the specified target server.
    	/// </returns>
        const std::vector<IPEndPoint>& GetRemoteServerIPEndPts();
    	/// <summary>
    	/// Retrieves the designated IP address for the server, if set.
    	/// </summary>
    	/// <returns>
    	/// The designated IP address for the server.
    	/// </returns>
        std::string GetServerIPAddr() const;
    	/// <summary>
    	/// Retrieves the designated port for the server, if set.
    	/// </summary>
    	/// <returns>
    	/// The designated port for the server.
    	/// </returns>
        long GetPort() const;
    	/// <summary>
    	/// Retrieves the query path for the request.
    	/// </summary>
    	/// <returns>
    	/// The query path for the request.
    	/// </returns>
        std::string GetQueryPath() const;
    	/// <summary>
    	/// Retrieves a list of values of the headers with the specified key.
    	/// </summary>
    	/// <param name="Key">
    	/// The key associated with the headers to be retrieved.
    	/// </param>
    	/// <returns>
    	/// A list of values of headers with specified key.
    	/// </returns>
        std::vector<std::string> GetRequestHeader(const std::string& Key) const;
    	/// <summary>
    	/// Compiles all header keys and values into a package.
    	/// </summary>
    	/// <returns>
    	/// The packaged list of headers and values.
    	/// </returns>
        std::string GetRequestHeaderSection() const;
        //std::string GetRequestBodyString() const;
    	/// <summary>
    	/// Retrieves the body stream of the content, if set.
    	/// </summary>
    	/// <returns>
    	/// A pointer to an istream instance for the content body.
    	/// </returns>
        std::istream* GetRequestBodyStream() const;

    	/// <summary>
    	/// Will set the method for the request.
    	/// </summary>
    	/// <param name="Method">
    	/// The method to set.
    	/// </param>
        void SetMethod(const std::string& Method);
    	/// <summary>
    	/// Will set the URL for the request.
    	/// </summary>
    	/// <param name="URL">
    	/// The URL to set.
    	/// </param>
        void SetURL(const std::string& URL);
    	/// <summary>
    	/// Add an end point to the list of possible end points.
    	/// </summary>
    	/// <param name="IPEndPt">
    	/// The end point to add.
    	/// </param>
        void AddRemoteServerEndPt(IPEndPoint IPEndPt);
    	/// <summary>
    	/// Set the remote server IP address.
    	/// </summary>
    	/// <param name="IPAddr">
    	/// The IP address to set.
    	/// </param>
        void SetRemoteServerIPAddr(const std::string& IPAddr);
    	/// <summary>
    	/// Set the remote server port.
    	/// </summary>
    	/// <param name="Port">
    	/// The port to set.
    	/// </param>
        void SetRemoteServerPort(const long Port);

    	/// <summary>
    	/// Will add a header entry with the specified key to the list of headers.
    	/// Duplicate values for a single key are allowed.
    	/// </summary>
    	/// <param name="Key">
    	/// The key to which the header value is to be associated.
    	/// </param>
    	/// <param name="Value">
    	/// The value of the header.
    	/// </param>
        void AddHeader(const std::string& Key, const std::string& Value);
    	/// <summary>
    	/// Will add a header entry with the specific key to the list of headers.
    	/// Any duplicate entries for the specified key will be removed.
    	/// </summary>
    	/// <param name="Key">
    	/// The key to which the header value is to be associated.
    	/// </param>
    	/// <param name="Value">
    	/// The value of the header.
    	/// </param>
        void SetHeader(const std::string& Key, const std::string& Value);
    	/// <summary>
    	/// Will set the specified HttpByteContent to be the body of the request.
    	/// </summary>
    	/// <param name="Content">
    	/// A pointer to the instance of HttpByteContent to set.
    	/// </param>
        void SetByteContent(const HttpByteContent* const Content);
    	/// <summary>
    	/// Will set the specified HttpByteContent to be the body of the request.
    	/// </summary>
    	/// <param name="Content">
    	/// A reference to the instance of HttpByteContent to set.
    	/// </param>
        void SetByteContent(const HttpByteContent& Content);
    	/// <summary>
    	/// Will set the specified HttpStringContent to be the body of the request.
    	/// </summary>
    	/// <param name="Content">
    	/// A pointer to the instance of HttpStringContent to set.
    	/// </param>
        void SetStringContent(const HttpStringContent* const Content);
    	/// <summary>
    	/// Will set the specified HttpStringContent to be the body of the request.
    	/// </summary>
    	/// <param name="Content">
    	/// A reference to the instance of HttpStringContent to set.
    	/// </param>
        void SetStringContent(const HttpStringContent& Content);
    	/// <summary>
    	/// Will set the specified HttpFileStreamContent to be the body of the request.
    	/// </summary>
    	/// <param name="Content">
    	/// A pointer to the instance of HttpFileStreamContent to set.
    	/// </param>
        void SetStreamContent(const HttpFileStreamContent* const Content);
    	/// <summary>
    	/// Will set the specified HttpFileStreamContent to be the body of the request.
    	/// </summary>
    	/// <param name="Content">
    	/// A reference to the instance of HttpFileStreamContent to set.
    	/// </param>
        void SetStreamContent(const HttpFileStreamContent& Content);
        
    // Private methods
    private:
    	/// <summary>
    	/// Will parse the specified URL and determine the protocol, server name, port, and query string.
    	/// </summary>
        void DecodeURL();
    	/// <summary>
    	/// Will trim the leading and trailing whitespace of the supplied string.
    	/// </summary>
    	/// <param name="Str">
    	/// The string to trim.
    	/// </param>
    	/// <returns>
    	/// The trimmed string.
    	/// </returns>
        std::string Trim(const std::string& Str) const;
        
        
    // Private instance variables
    private:
    	/// <summary>
    	/// The method of the request.
    	/// </summary>
        std::string mMethod;
    	/// <summary>
    	/// The URL of the request.
    	/// </summary>
        std::string mURL;
    	/// <summary>
    	/// The default protocol, in the case that the protocol is undefined.
    	/// </summary>
        std::string mDefaultProtocol;
    	/// <summary>
    	/// The protocol of the request.
    	/// </summary>
        std::string mProtocol;
    	/// <summary>
    	/// The host name of the remote target.
    	/// </summary>
        std::string mServerHostName;
    	/// <summary>
    	/// A list of possible end points for the remote server.
    	/// </summary>
        std::vector<IPEndPoint> mServerIPEndPts;
    	/// <summary>
    	/// The IP address of the remote server.
    	/// </summary>
        std::string mServerIPAddr;
    	/// <summary>
    	/// The port for the request.
    	/// </summary>
        long mServerPort;
    	/// <summary>
    	/// The query string for the request.
    	/// </summary>
        std::string mQueryPath;
    	/// <summary>
    	/// A list of all non-duplicated headers.
    	/// </summary>
        std::map<const std::string, std::string> mSingleHeaders;
    	/// <summary>
    	/// A list of headers which are potentially duplicated.
    	/// </summary>
        std::multimap<const std::string, std::string> mMultiHeaders;
    	/// <summary>
    	/// A pointer to the content body of the request.
    	/// </summary>
        HttpContent* mBody;
    };
}

#endif // HTTPREQUESTMESSAGE_H
