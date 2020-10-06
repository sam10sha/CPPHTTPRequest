#ifndef HTTPSTRINGCONTENT_H
#define HTTPSTRINGCONTENT_H

#include "HttpContent.h"

namespace Network
{
	/// <summary>
	/// Derived class from HttpContent specifying a string as the data source for the HTTP content stream.
	/// </summary>
    class HttpStringContent : public HttpContent
    {
    // Private instance variables
    private:
		/// <summary>
		/// The string to be used as the source of the stream content.
		/// </summary>
        std::string mStringContent;
        
    // Public construction/destruction
    public:
		/// <summary>
		/// Default constructor for the HttpStringContent class.
		/// Will set the string to empty and the content length to 0.
		/// </summary>
        HttpStringContent();
		/// <summary>
		/// Constructor for HttpStringContent class.
		/// Copies provided string to internal buffer to be used as source for stream content.
		/// </summary>
		/// <param name="Content">
		/// The string whose content is to be used as source for stream content.
		/// </param>
        HttpStringContent(const std::string& Content);
		/// <summary>
		/// Copy constructor for the HttpStringContent class.
		/// Will copy the properties of the HttpStringContent instance provided.
		/// </summary>
		/// <param name="Content">
		/// A reference to the instance of the HttpStringContent class to copy.
		/// </param>
        HttpStringContent(const HttpStringContent& Content);
        
    // Public operator overloads
    public:
		/// <summary>
		/// Overloaded equals operator. Will copy properties of specified instance of HttpStringContent class.
		/// </summary>
		/// <param name="Content">
		/// A reference to the instance of the HttpStringContent class to copy.
		/// </param>
		/// <returns>
		/// A reference to the current instance of the HttpStringContent class.
		/// </returns>
        HttpStringContent& operator=(const HttpStringContent& Content);
    };
}

#endif // HTTPSTRINGCONTENT_H
