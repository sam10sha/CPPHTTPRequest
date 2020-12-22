#ifndef HTTPRESPONSEMESSAGE_H
#define HTTPRESPONSEMESSAGE_H

#include <map>
#include "HttpStringContent.h"

namespace Network
{
	/// <summary>
	///
	/// </summary>
    class HttpResponseMessage
    {
    // Public construction/destruction
    public:
    	/// <summary>
    	/// Default constructor for the HttpResponseMessage.
    	/// Sets the method, protocol, server name, port and query string to default values.
    	/// </summary>
        HttpResponseMessage();
    	/// <summary>
    	/// Constructor for the HttpResponseMessage class.
    	/// </summary>
    	/// <param name="Method"></param>
    	/// <param name="URL"></param>
        HttpResponseMessage(const std::string& Method, const std::string& URL);
    	/// <summary>
    	/// Copy constructor for the HttpResponseMessage class.
        /// Deleted function. Copying instances of this class is not permitted.
    	/// </summary>
    	/// <param name="ResponseMsg"></param>
        HttpResponseMessage(const HttpResponseMessage& ResponseMsg) = delete;
    	/// <summary>
    	/// Destructor for the HttpResponseMessage class.
    	/// Will deallocate memory for the response content, if set and managed internally.
    	/// </summary>
        ~HttpResponseMessage();
		
    // Public operator overloads
    public:
    	/// <summary>
    	/// Overloaded equals operator. Will copy properties of specified instance of HttpResponseMessage class.
		/// Deleted function. Copying instances of this class is not permitted.
    	/// </summary>
    	/// <param name="ResponseMsg">
    	/// A reference to the instance of HttpResponseMessage to copy.
    	/// </param>
        void operator=(const HttpResponseMessage& ResponseMsg) = delete;
        
    // Public methods
    public:
    	/// <summary>
    	/// Will retrieve the response status code.
    	/// </summary>
    	/// <returns>
    	/// The response status code.
    	/// </returns>
        long GetResponseStatusCode() const;
    	/// <summary>
    	/// Will retrieve the response status message.
    	/// </summary>
    	/// <returns>
    	/// The response status message.
    	/// </returns>
        std::string GetResponseStatusMessage() const;
    	/// <summary>
    	/// Will compile and retrieve a list containing the keys and values of all headers.
    	/// </summary>
    	/// <returns>
    	/// The list of all headers.
    	/// </returns>
        std::multimap<const std::string, std::string> GetAllHeaders() const;
    	/// <summary>
    	/// Retrieves a list of values of the headers with the specified key.
    	/// </summary>
    	/// <param name="HeaderKey">
    	/// The key associated with the headers to be retrieved.
    	/// </param>
    	/// <returns>
    	/// A list of values of headers with specified key.
    	/// </returns>
        std::vector<std::string> GetResponseHeader(const std::string& HeaderKey) const;
    	/// <summary>
    	/// Will retrieve the content body of the response as a string.
    	/// </summary>
    	/// <returns>
    	/// A string containing the content body of the response.
    	/// </returns>
        std::string GetStringContentBody() const;
    	/// <summary>
    	/// Will retrieve the content body of the response as a stream.
    	/// </summary>
    	/// <returns>
    	/// A pointer to an instance of istream from which bytes of the response can be retrieved.
    	/// </returns>
        std::istream* GetStreamContentBody() const;

    	/// <summary>
    	/// Will set the method of the request.
    	/// </summary>
    	/// <param name="Method">
    	/// The method to set.
    	/// </param>
        void SetMethod(const std::string& Method);
    	/// <summary>
    	/// Will set the URL of the request.
    	/// </summary>
    	/// <param name="URL">
    	/// The URL to set.
    	/// </param>
        void SetURL(const std::string& URL);
    	/// <summary>
    	/// Will set the host name of the remote server.
    	/// </summary>
    	/// <param name="ServerHostName">
    	/// The host name of the remote server.
    	/// </param>
        void SetServerHostName(const std::string& ServerHostName);
    	/// <summary>
    	/// Will set the server IP address of the remote server.
    	/// </summary>
    	/// <param name="ServerIPAddr">
    	/// The IP address of the server.
    	/// </param>
        void SetServerIPAddr(const std::string& ServerIPAddr);
    	/// <summary>
    	/// Will set the port of the remote server.
    	/// </summary>
    	/// <param name="Port">
    	/// The port for the request.
    	/// </param>
        void SetPort(const long Port);
    	/// <summary>
    	/// Will set the query path for the response.
    	/// </summary>
    	/// <param name="QueryPath">
    	/// The query path of the response.
    	/// </param>
        void SetQueryPath(const std::string& QueryPath);
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
    	/// Will parse the raw header from the response and populate the lists of headers.
    	/// </summary>
    	/// <param name="Header">
    	/// The headers from the response.
    	/// </param>
        void ParseRawHeader(const std::string& Header);
    	/// <summary>
    	/// Will set the string content as the response content.
    	/// </summary>
    	/// <param name="Content">
    	/// A pointer to the instance of HttpStringContent to set.
    	/// </param>
        void SetStringContent(const HttpStringContent* const Content);
    	/// <summary>
    	/// Will set the string content as the response content.
    	/// </summary>
    	/// <param name="Content">
    	/// A reference to the instance of HttpStringContent to set.
    	/// </param>
        void SetStringContent(const HttpStringContent& Content);
    	/// <summary>
    	/// Will set the response content.
    	/// </summary>
    	/// <param name="Content">
    	/// A pointer to the HttpContent to set.
    	/// </param>
    	/// <param name="IsManagedExternally">
    	/// A flag indicating whether the memory for the specified HttpContent is managed
    	/// externally.
    	/// If not managed externally, the memory for the HttpContent will be deallocated upon
    	/// the destruction of the current instance of HttpResponseMessage.
    	/// </param>
        void SetContent(HttpContent* const Content, const bool IsManagedExternally = false);
	
    // Private methods
    private:
    	/// <summary>
    	/// Will parse the response header status line for the response status code, response
    	/// status message, and protocol version.
    	/// </summary>
    	/// <param name="HeaderStatusLine">
    	/// The header status line to parse.
    	/// </param>
        void ParseResponseHeaderStatusLine(const std::string& HeaderStatusLine);
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
    	/// The protocol version of the request.
    	/// </summary>
        std::string mProtocolVersion;
    	/// <summary>
    	/// The response status code.
    	/// </summary>
        long mStatusCode;
    	/// <summary>
    	/// The response status message.
    	/// </summary>
        std::string mStatusMessage;
    	/// <summary>
    	/// The method of the request.
    	/// </summary>
        std::string mMethod;
    	/// <summary>
    	/// The URL of the request.
    	/// </summary>
        std::string mURL;
    	/// <summary>
    	/// The protocol of the request.
    	/// </summary>
        std::string mProtocol;
    	/// <summary>
    	/// The host name of the remote target.
    	/// </summary>
        std::string mServerHostName;
    	/// <summary>
    	/// The IP address of the remote server.
    	/// </summary>
        std::string mServerIPAddress;
    	/// <summary>
    	/// The port for the request.
    	/// </summary>
        long mPort;
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
    	/// <summary>
    	/// A flag indicating whether the memory allocated by the content body is managed externally.
    	/// If not managed externally, the memory will be deallocated upon the destruction of the current instance of
    	/// HttpResponseMessage.
    	/// </summary>
        bool BodyManagedExternally;
    };
}

#endif // HTTPRESPONSEMESSAGE_H
