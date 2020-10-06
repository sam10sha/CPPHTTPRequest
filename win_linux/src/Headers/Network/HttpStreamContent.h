#ifndef HTTPSTREAMCONTENT_H
#define HTTPSTREAMCONTENT_H

#include "HttpContent.h"
#include "GenericIStreamBuf.h"
#include <iostream>

namespace Network
{
	/// <summary>
	/// Derived class from HttpContent specifying another stream as the data source for the HTTP content stream.
	/// </summary>
	class HttpStreamContent : public HttpContent
	{
	// Private member variables
	private:
		/// <summary>
		/// The generic input stream buffer used for the content stream.
		/// </summary>
		GenericIO::GenericIStreamBuf IStreamBuf;

	// Public construction
	public:
		/// <summary>
		/// Constructor for the HttpStreamContent.
		/// Initializes the stream buffer and content stream.
		/// </summary>
		/// <param name="BufLen">
		/// The buffer size of the generic stream buffer.
		/// </param>
		/// <param name="ContentLength">
		/// The expected number of bytes of the entire content stream.
		/// </param>
		HttpStreamContent(const size_t BufLen, const size_t ContentLength);

	// Public member functions
	public:
		/// <summary>
		/// Directly inject bytes of data into the content stream.
		/// </summary>
		/// <param name="Data">
		/// A pointer to a byte array containing the bytes to inject.
		/// </param>
		/// <param name="Length">
		/// The number of bytes to inject from the byte array.
		/// </param>
		void InjectData(const void* const Data, const size_t Length);
		/// <summary>
		/// Registers a callback function to specify a method of retrieving additional data when the stream
		/// buffer contains available space.
		/// </summary>
		/// <param name="DataRetrieveCB">
		/// The callback structure to register.
		/// </param>
		void RegisterDataCB(GenericIO::GenericIStreamBuf::DataRetrieveCallback_base* const DataRetrieveCB);
	};
}

#endif // HTTPSTREAMCONTENT_H
