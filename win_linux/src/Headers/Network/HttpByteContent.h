#ifndef HTTPBYTECONTENT_H
#define HTTPBYTECONTENT_H

#include "HttpContent.h"
#include "GenericStreamBuf.h"

namespace Network
{
	/// <summary>
	/// Derived class from HttpContent storing a byte array as the data source for the HTTP content stream.
	/// </summary>
    class HttpByteContent : public HttpContent
    {
    // Private embedded declarations
    private:
        /* class membuf : public std::basic_streambuf<char>
        {
		public:
			membuf(const char *p, size_t l)
			{
				setg((char*)p, (char*)p, (char*)p + l);
			}
        };
        class memstream : public std::istream
        {
        public:
            memstream(const char *p, size_t l) :
                std::istream(&_buffer),
                _buffer(p, l)
            {
                rdbuf(&_buffer);
            }

        private:
            membuf _buffer;
        }; */
        
        
        
    // Private instance variables
    private:
		/// <summary>
		/// A pointer to the byte array to be used as the content.
		/// </summary>
        char* mContentBytes;
		/// <summary>
		/// The generic stream buffer to be used by the istream object.
		/// </summary>
        GenericIO::GenericStreamBuf* mContentStreamBuf;
        
    // Public construction/destruction
    public:
		/// <summary>
		/// Default constructor for HttpByteContent class.
		/// Sets pointer to byte array to nullptr and content length to 0.
		/// </summary>
        HttpByteContent();
		/// <summary>
		/// Constructor for HttpByteContent class.
		/// Accepts a predefined byte array as the source of content. Makes a copy of the contents of the byte
		/// array and stores it as its content source.
		/// </summary>
		/// <param name="Content">
		/// A pointer to a byte array to be used as content for content stream.
		/// </param>
		/// <param name="ContentLength">
		/// The size of the byte array.
		/// </param>
        HttpByteContent(const void* const Content, const size_t ContentLength);
		/// <summary>
		/// Copy constructor for the HttpByteContent class.
		/// Will construct a new byte array and copy contents of the byte array of the provided HttpByteContent instance
		/// to a new array.
		/// </summary>
		/// <param name="Content">
		/// A reference to the instance of the HttpByteContent class to copy.
		/// </param>
        HttpByteContent(const HttpByteContent& Content);
		/// <summary>
		/// Destructor for the HttByteContent class.
		/// Will deallocate the memory used by the byte array.
		/// </summary>
        ~HttpByteContent();
        
    // Public operator overloads
    public:
		/// <summary>
		/// Overloaded equals operator. Will copy properties of specified instance of HttpByteContent class.
		/// </summary>
		/// <param name="Content">
        /// A reference to the instance of the HttpByteContent class to copy.
        /// </param>
		/// <returns>
		/// A reference to the current instance of the HttpByteContent class.
		/// </returns>
        HttpByteContent& operator=(const HttpByteContent& Content);
    };
}

#endif // HTTPBYTECONTENT_H
