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
    //TestDynamic(argc, argv);
	//TestStatic(argc, argv);
	TestStatic_2(argc, argv);
	return 0;
}

void TestDynamic(int argc, char** argv)
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
}

void TestStatic(int argc, char** argv)
{
	Network::HttpRequestMessage RequestMsg("POST", "http://httpbin.org/post");
	RequestMsg.SetHeader("Content-Type", "text/plain");
	RequestMsg.SetHeader("Content-Length", "11");
	RequestMsg.SetHeader("Connection", "close");
	RequestMsg.SetHeader("Accept", "*/*");
	RequestMsg.SetByteContent(Network::HttpByteContent("Hello world", 12));
	
	Network::WebClient Client;
	std::string Response = Client.SendRequest(RequestMsg);
	
	std::ofstream output("output.txt", std::ofstream::out | std::ofstream::trunc);
	output.write(Response.c_str(), (std::streamsize)Response.length());
	output.close();
}
void TestStatic_2(int argc, char** argv)
{
	Network::HttpRequestMessage RequestMsg("POST", "http://httpbin.org/post");
	Network::HttpResponseMessage ResponseMsg;
	RequestMsg.SetHeader("Content-Type", "text/plain");
	RequestMsg.SetHeader("Content-Length", "11");
	RequestMsg.SetHeader("Connection", "close");
	RequestMsg.SetHeader("Accept", "*/*");
	RequestMsg.SetStringContent(Network::HttpStringContent(std::string("Hello world")));
	
	Network::WebClient Client;
	std::cout << "driver[TestStatic_2]: Starting WebClient" << std::endl;
	Client.SendRequest_2(RequestMsg, ResponseMsg);
	std::cout << "driver[TestStatic_2]: Completed WebClient" << std::endl;
	/* std::string Response = ResponseMsg.GetStringContentBody();
	
	std::ofstream output("output.txt", std::ofstream::out | std::ofstream::trunc);
	output.write(Response.c_str(), (std::streamsize)Response.length());
	output.close(); */
}

