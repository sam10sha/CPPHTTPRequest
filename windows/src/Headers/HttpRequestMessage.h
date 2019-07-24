#ifndef HTTPREQUESTMESSAGE_H
#define HTTPREQUESTMESSAGE_H

#include <map>
#include <string>

namespace Networking
{
	class HttpRequestMessage
	{
	private:
		struct IPEndPoint
		{
			const std::string IPAddr;
			const long Port;
		};
		
		
	// Public construction
	public:
		HttpRequestMessage();
		HttpRequestMessage(const std::string& Method, const std::string& URL);
		
	// Public methods
	public:
		std::string GetMethod() const;
		std::string GetURL() const;
		std::string GetProtocol() const;
		std::string GetServerHostName() const;
		std::string GetServerIPAddress() const;
		long GetPort() const;
		std::string GetQueryPath() const;
		std::string GetRequestHeader() const;
		std::string GetRequestBody() const;
		std::string GetRequest() const;
		
		void SetMethod(const std::string& Method);
		void SetURL(const std::string& URL);
		void SetHeader(const std::string& Key, const std::string& Value);
		void SetBody(const std::string& Value);
		
	// Private methods
	private:
		void DecodeURL();
		void ResolveIPAddrs(std::vector<IPEndPoint> IPAddrs);
		
		
	// Private instance variables
	private:
		std::string mMethod;
		std::string mURL;
		std::string mDefaultProtocol;
		std::string mProtocol;
		std::string mServerHostName;
		std::string mServerIPAddress;
		long mPort;
		std::string mQueryPath;
		std::map<const std::string, std::string> mHeaders;
		std::string mBody;
	};
}

#endif // HTTPREQUESTMESSAGE_H