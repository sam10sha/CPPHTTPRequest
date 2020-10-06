#ifndef HTTPCONTENT_H
#define HTTPCONTENT_H

#include <iostream>

namespace Network
{
	/// <summary>
	/// Base class for storing content streams for HTTP connections.
	/// </summary>
    class HttpContent
    {
        // Private instance variables
        protected:
    		/// <summary>
			/// A pointer to the content stream if set.
			/// </summary>
            std::istream* mContentStream;
    		/// <summary>
			/// The total number of bytes to be retrieved from the stream.
			/// </summary>
            size_t mContentLength;
            
        // Protected construction
        protected:
    		/// <summary>
			/// Default constructor for HttpContent.
			/// Sets content stream pointer to nullptr with 0 content length.
			/// </summary>
            HttpContent();
    		/// <summary>
			/// Constructor for HttpContent.
			/// Will set the content stream and content length to the respective values.
			/// </summary>
			/// <param name="Content">
			/// A pointer to an instance of istream.
			/// </param>
			/// <param name="ContentLength">
			/// The number of bytes in the stream.
			/// </param>
            HttpContent(std::istream* const Content, const size_t ContentLength);
    		/// <summary>
			/// Copy constructor for the HttpContent class.
			/// Deleted function. Copying instances of this class is not permitted.
			/// </summary>
			/// <param name="Content">
			/// A reference to the instance of the HttpContent class to copy.
			/// </param>
            HttpContent(const HttpContent& Content) = delete;
        // Public destruction
        public:
    		/// <summary>
			/// Destructor for the HttpContent class.
			/// Will deallocate the memory used by the content stream, if specified.
			/// </summary>
            virtual ~HttpContent();
        
        // Public operator overloads
        public:
    		/// <summary>
			/// Overloaded equals operator. Will copy properties of specified instance of HttpContent class.
			/// Deleted function. Copying instances of this class is not permitted.
			/// </summary>
			/// <param name="Content">
			/// A reference to the instance of the HttpContent class to copy.
			/// </param>
            void operator=(const HttpContent& Content) = delete;
            
        // Public methods            
        public:
    		/// <summary>
			/// Retrieves the instance of istream associated with the content stream.
			/// </summary>
			/// <returns>
			/// An instance of istream.
			/// </returns>
            virtual std::istream* GetContent();
    		/// <summary>
			/// Retrieves the length of the content stream.
			/// </summary>
			/// <returns>
			/// The length of the content stream.
			/// </returns>
            virtual size_t GetContentLength() const;
    };
}

#endif // HTTPCONTENT_H

