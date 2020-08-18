#ifndef HTTPSOCKETSTREAMCONTENT_H
#define HTTPSOCKETSTREAMCONTENT_H

#include "HttpContent.h"
#include "GenericIStreamBuf.h"
#include <iostream>

namespace Network
{
	class HttpStreamContent : public HttpContent
	{
	// Private member variables
	private:
		GenericIO::GenericIStreamBuf IStreamBuf;

	// Public construction
	public:
		HttpStreamContent(const size_t BufLen, const size_t ContentLength);

	// Public member functions
	public:
		void InjectData(const void* const Data, const size_t Length);
		void RegisterDataCB(GenericIO::GenericIStreamBuf::DataRetrieveCallback_base* const DataRetrieveCB);
	};
}

#endif // HTTPSOCKETSTREAMCONTENT_H
