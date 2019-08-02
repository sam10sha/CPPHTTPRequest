#ifndef HTTPREQUESTMESSAGE_H
#define HTTPREQUESTMESSAGE_H

#include <map>
#include <string>
#include <vector>
#include "HttpContent.h"
#include "HttpByteContent.h"
#include "HttpFileStreamContent.h"
#include "HttpStringContent.h"

namespace Network
{
    class HttpRequestMessage
    {
    // Private structures
    private:
        struct IPEndPoint
        {
            std::string mIPAddr;
            long mPort;
            
            IPEndPoint(const std::string& IPAddr, const long Port) :
                mIPAddr(IPAddr),
                mPort(Port) { }

			void operator=(const IPEndPoint& EndPt)
			{
				mIPAddr.clear();
				mIPAddr.append(EndPt.mIPAddr);
				mPort = EndPt.mPort;
			}
        };
        
        
    // Public construction/destruction
    public:
        HttpRequestMessage();
        HttpRequestMessage(const std::string& Method, const std::string& URL);
        ~HttpRequestMessage();
        
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
        std::string GetRequestBodyString() const;
        std::string GetRequest() const;
        
        void SetMethod(const std::string& Method);
        void SetURL(const std::string& URL);
        void SetHeader(const std::string& Key, const std::string& Value);
        void SetByteContent(const HttpByteContent* const Content);
        void SetByteContent(const HttpByteContent& Content);
        void SetStringContent(const HttpStringContent* const Content);
        void SetStringContent(const HttpStringContent& Content);
        void SetStreamContent(const HttpFileStreamContent* const Content);
        void SetStreamContent(const HttpFileStreamContent& Content);
        
    // Private methods
    private:
        void DecodeURL();
        void SetRemoteServerIPAddr();
		bool ResolveIPAddrs(std::vector<IPEndPoint>& IPAddrs);
        
        
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
        HttpContent* mBody;
    };
}

#endif // HTTPREQUESTMESSAGE_H
