#ifndef HTTPREQUESTMESSAGE_H
#define HTTPREQUESTMESSAGE_H

#include <map>
#include <string>
#include <vector>
#include "HttpContent.h"
#include "HttpByteContent.h"
#include "HttpFileStreamContent.h"
#include "HttpStringContent.h"
#include "IStreamWrap.h"

namespace Network
{
    class HttpRequestMessage
    {
    // Public structures
    public:
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
            HttpRequestMessage(const HttpRequestMessage& RequestMsg) = delete;
        ~HttpRequestMessage();
        	
    // Public operator overloads
    public:
        void operator=(const HttpRequestMessage& RequestMsg) = delete;
        
    // Public methods
    public:
        std::string GetMethod() const;
        std::string GetURL() const;
        std::string GetProtocol() const;
        std::string GetServerHostName() const;
        const std::vector<IPEndPoint>& GetRemoteServerIPEndPts();
        std::string GetServerIPAddr() const;
        long GetPort() const;
        std::string GetQueryPath() const;
        std::string GetRequestHeader(const std::string& Key) const;
        std::string GetRequestHeaderSection() const;
        //std::string GetRequestBodyString() const;
        void GetRequestBodyStream(IStreamWrap& Stream) const;
        
        void SetMethod(const std::string& Method);
        void SetURL(const std::string& URL);
        void AddRemoteServerEndPt(IPEndPoint IPEndPt);
        void SetRemoteServerIPAddr(const std::string& IPAddr);
        void SetRemoteServerPort(const long Port);
        
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
        
        
    // Private instance variables
    private:
        std::string mMethod;
        std::string mURL;
        std::string mDefaultProtocol;
        std::string mProtocol;
        std::string mServerHostName;
        std::vector<IPEndPoint> mServerIPEndPts;
        std::string mServerIPAddr;
        long mServerPort;
        std::string mQueryPath;
        std::map<const std::string, std::string> mHeaders;
        HttpContent* mBody;
    };
}

#endif // HTTPREQUESTMESSAGE_H
