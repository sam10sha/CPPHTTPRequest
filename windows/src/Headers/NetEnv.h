#ifndef NETENV_H
#define NETENV_H

#include <WinSock2.h>

namespace Network
{
	class NetEnv
	{
	// Public static instance variables
	public:
		static NetEnv Instance;

	// Public construction/destruction
	public:
		NetEnv();
		~NetEnv();

	// Public member functions
	public:
		bool IsInitialized();
		bool Initialize();
		bool CleanUp();

	// Private instance variables
	private:
		bool mIsInitialized;
	};
}

#endif // NETENV_H