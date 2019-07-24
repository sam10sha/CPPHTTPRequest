#include <stdafx.h>
#include "HttpRequestMessage.h"

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	Networking::HttpRequestMessage RequestMsg("GET", "http://update.amadasoftware.com/customerdataportal/api/test");
	RequestMsg.SetHeader("Content-Type", "application/json");
	RequestMsg.SetBody("Hello world");
	
	cout << RequestMsg.GetRequest() << endl;
	
	return 0;
}