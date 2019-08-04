#include "stdafx.h"
#include "NetEnv.h"

// Public static instance variables
Network::NetEnv Network::NetEnv::Instance;



// Public construction
Network::NetEnv::NetEnv()
	: mIsInitialized(false)
{
	Initialize();
}
Network::NetEnv::~NetEnv()
{
	CleanUp();
}


// Public member functions
bool Network::NetEnv::IsInitialized()
{
	return mIsInitialized;
}
bool Network::NetEnv::Initialize()
{
	WSADATA wsaData;
    int SocketStartupError = 0;
	if(mIsInitialized == false)
	{
		SocketStartupError = WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	return SocketStartupError == 0;
}
bool Network::NetEnv::CleanUp()
{
	int SocketCleanupError;
	SocketCleanupError = WSACleanup();
	return SocketCleanupError == 0;
}