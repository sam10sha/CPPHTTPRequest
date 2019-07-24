#include <stdafx.h>
#include "HttpByteContent.h"
#include "HttpStringContent.h"
#include "HttpRequestMessage.h"
#include "WebClient.h"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    if(argc >= 3)
    {
        //Network::HttpRequestMessage RequestMsg("GET", "http://update.amadasoftware.com/customerdataportal/api/test");
        Network::HttpRequestMessage RequestMsg(argv[1], argv[2]);
        RequestMsg.SetHeader("Content-Type", "text/plain");
        RequestMsg.SetHeader("Content-Length", "11");
        RequestMsg.SetHeader("Connection", "close");
        RequestMsg.SetHeader("Accept", "*/*");
        RequestMsg.SetByteContent(Network::HttpByteContent("Hello world", 12));
        
        //cout << RequestMsg.GetRequest() << endl;
        
        Network::WebClient Client;
        std::string Response = Client.SendRequest(RequestMsg);
        
        std::cout << Response << std::endl;
    }
    else
    {
        cout << "Usage: " << *argv << " [Method] [URL]" << endl;
    }
    return 0;
}
