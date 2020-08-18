/*
 * HttpSocketStreamContent.cpp
 *
 *  Created on: Jul 20, 2020
 *      Author: saumil
 */

#include "HttpStreamContent.h"
#include "stdafx.h"
#include "HttpContent.h"

// Public construction
Network::HttpStreamContent::HttpStreamContent(const size_t BufLen, const size_t ContentLength) :
	HttpContent(nullptr, 0),
	IStreamBuf(BufLen)
{
	mContentStream = new std::istream(&IStreamBuf);
	mContentLength = ContentLength;
}

// Public member functions
void Network::HttpStreamContent::InjectData(const void* const Data, const size_t Length)
{
	IStreamBuf.sputn((const char*)Data, Length);
}
void Network::HttpStreamContent::RegisterDataCB(GenericIO::GenericIStreamBuf::DataRetrieveCallback_base* const DataRetrieveCB)
{
	IStreamBuf.RegisterOnDataRetrieveCallback(DataRetrieveCB);
}
