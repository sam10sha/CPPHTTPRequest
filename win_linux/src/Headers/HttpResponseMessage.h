#ifndef HTTPRESPONSEMESSAGE_H
#define HTTPRESPONSEMESSAGE_H

#include "HttpStringContent.h"

namespace Network
{
    class HttpResponseMessage
    {
    // Public construction/destruction
    public:
        HttpResponseMessage();
        HttpResponseMessage(const std::string& Method, const std::string& URL);
        ~HttpResponseMessage();
        
    // Public methods
    public:
        std::string GetRequestHeader(const std::string& HeaderKey) const;
        std::string GetStringContentBody() const;
        std::istream* GetStreamContentBody() const;
        
        void SetHeader(const std::string& Key, const std::string& Value);
        void SetStringContent(const HttpStringContent* const Content);
        void SetStringContent(const HttpStringContent& Content);
        
    // Private instance variables
    private:
        std::string mProtocolVersion;
        long mStatusCode;
        std::string mStatusMessage;
        std::string mMethod;
        std::string mURL;
        std::string mProtocol;
        std::string mServerHostName;
        std::string mServerIPAddress;
        long mPort;
        std::string mQueryPath;
        std::map<const std::string, std::string> mHeaders;
        HttpContent* mBody;
    };
}

#endif // HTTPRESPONSEMESSAGE_H
