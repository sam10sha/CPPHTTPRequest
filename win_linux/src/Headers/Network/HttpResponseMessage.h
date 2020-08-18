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
        HttpResponseMessage(const HttpResponseMessage& ResponseMsg) = delete;
        ~HttpResponseMessage();
		
    // Public operator overloads
    public:
        void operator=(const HttpResponseMessage& ResponseMsg) = delete;
        
    // Public methods
    public:
        long GetResponseStatusCode() const;
        std::string GetResponseStatusMessage() const;
        std::multimap<const std::string, std::string> GetAllHeaders() const;
        std::vector<std::string> GetResponseHeader(const std::string& HeaderKey) const;
        std::string GetStringContentBody() const;
        std::istream* GetStreamContentBody() const;
        
        void SetMethod(const std::string& Method);
        void SetURL(const std::string& URL);
        void SetServerHostName(const std::string& ServerHostName);
        void SetServerIPAddr(const std::string& ServerIPAddr);
        void SetPort(const long Port);
        void SetQueryPath(const std::string& QueryPath);
        void AddHeader(const std::string& Key, const std::string& Value);
        void SetHeader(const std::string& Key, const std::string& Value);
        void ParseRawHeader(const std::string& Header);
        void SetStringContent(const HttpStringContent* const Content);
        void SetStringContent(const HttpStringContent& Content);
        void SetContent(HttpContent* const Content, const bool IsManagedExternally = false);
	
    // Private methods
    private:
        void ParseResponseHeaderStatusLine(const std::string& HeaderStatusLine);
        std::string Trim(const std::string& Str) const;
        
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
        std::map<const std::string, std::string> mSingleHeaders;
        std::multimap<const std::string, std::string> mMultiHeaders;
        HttpContent* mBody;
        bool BodyManagedExternally;
    };
}

#endif // HTTPRESPONSEMESSAGE_H
