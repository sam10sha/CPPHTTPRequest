#ifndef HTTPFILESTREAMCONTENT_H
#define HTTPFILESTREAMCONTENT_H

#include "HttpContent.h"

namespace Network
{
	/// <summary>
	/// Derived class from HttpContent specifying a file as the data source for the HTTP content stream.
	/// </summary>
    class HttpFileStreamContent : public HttpContent
    {
    // Private instance variables
    private:
		/// <summary>
		/// The path of the file to be used as the source of content.
		/// </summary>
        std::string mFileName;
        
    // Public construction/destruction
    public:
		/// <summary>
		/// Default constructor for the HttpFileStreamContent class.
		/// Will set the file path to an empty string and the content length to 0.
		/// </summary>
        HttpFileStreamContent();
		/// <summary>
		/// Constructor for the HttpFileContentStream.
		/// Accepts a path to the file to be used as the source of content.
		/// </summary>
		/// <param name="FileName">
		/// The path of the file to be used as the source of content.
		/// </param>
        HttpFileStreamContent(const std::string& FileName);
		/// <summary>
		/// Copy constructor for the HttpFileStreamContent class.
		/// Will copy the properties of the HttpFileStreamContent instance provided.
		/// </summary>
		/// <param name="Content">
		/// A reference to the instance of the HttpFileStreamContent class to copy.
		/// </param>
        HttpFileStreamContent(const HttpFileStreamContent& Content);
		/// <summary>
		/// Destructor for the HttpFileStreamContent class.
		/// Will close any open/available file streams associated with the content stream.
		/// </summary>
        ~HttpFileStreamContent();
        
    // Public operator overloads
    public:
		/// <summary>
		/// Overloaded equals operator. Will copy properties of specified instance of HttpFileStreamContent class.
		/// </summary>
		/// <param name="Content">
		/// A reference to the instance of the HttpFileStreamContent class to copy.
		/// </param>
		/// <returns>
		/// A reference to the current instance of the HttpFileStreamContent class.
		/// </returns>
        HttpFileStreamContent& operator=(const HttpFileStreamContent& Content);
    };
}

#endif // HTTPFILESTREAMCONTENT_H
