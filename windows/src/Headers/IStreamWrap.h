#ifndef ISTREAM_WRAP_H
#define ISTREAM_WRAP_H

#include <iostream>

namespace Network
{
	struct IStreamWrap
	{
		std::istream* mStream;
		
		IStreamWrap()
			: mStream(NULL) { }
		IStreamWrap(std::istream* Stream)
			: mStream(Stream) { }
		~IStreamWrap()
		{
			if(mStream)
			{
				delete mStream;
				mStream = NULL;
			}
		}
	};
}

#endif // ISTREAM_WRAP_H