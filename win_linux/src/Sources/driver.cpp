#include <stdafx.h>
#include "HttpByteContent.h"
#include "HttpStringContent.h"
#include "HttpRequestMessage.h"
#include "HttpResponseMessage.h"
#include "WebClient.h"

using std::cout;
using std::endl;

void TestDynamic(int argc, char** argv);
void TestStatic(int argc, char** argv);
void TestStatic_2(int argc, char** argv);

int main(int argc, char** argv)
{
    TestDynamic(argc, argv);
    //TestStatic(argc, argv);
    //TestStatic_2(argc, argv);
    return 0;
}


void TestDynamic(int argc, char** argv)
{
    if(argc >= 3)
    {
        //Network::HttpRequestMessage RequestMsg("GET", "http://update.amadasoftware.com/customerdataportal/api/test");
        Network::HttpRequestMessage RequestMsg(argv[1], argv[2]);
        Network::HttpResponseMessage ResponseMsg;
        RequestMsg.SetHeader("Content-Type", "text/plain");
        //RequestMsg.SetHeader("Content-Length", "11");
        RequestMsg.SetHeader("Connection", "close");
        RequestMsg.SetHeader("Accept", "*/*");
        //RequestMsg.SetByteContent(Network::HttpByteContent("Hello world", 12));
        
        //cout << RequestMsg.GetRequest() << endl;
        
        Network::WebClient Client;
        Client.SendRequest(RequestMsg, ResponseMsg);
        std::string Response = ResponseMsg.GetStringContentBody();
        
        cout << "driver[TestDynamic]: Response code: " << ResponseMsg.GetResponseStatusCode() << endl;
	
	std::ofstream output("output.txt", std::ofstream::out | std::ofstream::trunc);
	output.write(Response.c_str(), (std::streamsize)Response.length());
	output.close();
    }
    else
    {
        cout << "Usage: " << *argv << " [Method] [URL]" << endl;
    }
}
void TestStatic(int argc, char** argv)
{
	//Network::HttpRequestMessage RequestMsg("POST", "http://httpbin.org/post");
        //Network::HttpRequestMessage RequestMsg("POST", "https://swdownload.amada.comldkjfdl/fileservice/newsoftware");
        Network::HttpRequestMessage RequestMsg("POST", "http://swdownload.amada.com/fileservice/newsoftware");
	Network::HttpResponseMessage ResponseMsg;
	RequestMsg.SetHeader("Content-Type", "text/plain");
	RequestMsg.SetHeader("Content-Length", "11");
	RequestMsg.SetHeader("Connection", "close");
	RequestMsg.SetHeader("Accept", "*/*");
	RequestMsg.SetStringContent(Network::HttpStringContent(std::string("Hello world")));
	
	Network::WebClient Client;
	Client.SendRequest(RequestMsg, ResponseMsg);
	std::string Response;
	Response = std::to_string(ResponseMsg.GetResponseStatusCode()) + "\n";
	Response += ResponseMsg.GetStringContentBody();
	
	std::ofstream output("output.txt", std::ofstream::out | std::ofstream::trunc);
	output.write(Response.c_str(), (std::streamsize)Response.length());
	output.close();
}
void TestStatic_2(int argc, char** argv)
{
	Network::HttpRequestMessage RequestMsg("GET", "http://update.amadasoftware.com/ems/v76/ws/productKey/68269595-31d5-4a5e-8732-78034a0ea2a4/licenseretrieval.ws");
	Network::HttpResponseMessage ResponseMsg;
	
	RequestMsg.SetHeader(std::string("Connection"), std::string("Close"));
	
	Network::WebClient Client;
	Client.SendRequest(RequestMsg, ResponseMsg);
    
    
	// Writing output to file
	const size_t ResponseBufLen = 16;
	std::string Response;
	char ResponseBuf[ResponseBufLen];
	std::memset(ResponseBuf, 0, ResponseBufLen);
	
	Response = sprintf(ResponseBuf, "%d", (int)ResponseMsg.GetResponseStatusCode());
	Response += "\r\n";
	Response += ResponseMsg.GetStringContentBody();
	
	std::ofstream output("output.txt", std::ofstream::out | std::ofstream::trunc);
	output.write(Response.c_str(), (std::streamsize)Response.length());
	output.close();
}

