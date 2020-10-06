#ifndef GENERICISTREAMBUF_H
#define GENERICISTREAMBUF_H

#include "GenericStreamBuf.h"
#include <streambuf>

namespace GenericIO
{
	/// <summary>
	/// Input stream buffer, inheriting from GenericStreamBuf.
	/// Depends on specification of source to retrieve data for input streams.
	/// </summary>
    class GenericIStreamBuf : public GenericStreamBuf
    {
    // Public structures
    public:
        /// <summary>
        /// Callback structure specifying method of data retrieval for input stream buffer.
        /// </summary>
        struct DataRetrieveCallback_base
        {
        // Public member functions
        public:
            /// <summary>
            /// The primary operation to perform for the callback.
        	/// Performing this operation will attempt to fill the buffer with data.
            /// </summary>
            /// <param name="Buffer">
            /// A pointer to the buffer in which to store data.
            /// </param>
            /// <param name="MaxLen">
            /// The maximum amount of data to store into the buffer.
            /// </param>
        	/// <returns>
            /// The number of bytes written into the buffer.
            /// </returns>
            virtual size_t Invoke(void* const Buffer, const size_t MaxLen)
            {
                return 0;
            }
        };

        /// <summary>
        /// Callback structure for data retrieval for input stream buffer, inheriting from DataRetrieveCallback_base,
        /// designed for instances of classes to specify member functions as callbacks.
        /// </summary>
        /// <tparam name="T">
        /// The type which will be used for the callback class
        /// </tparam>
        template<class T>
        struct DataRetrieveCallback_member : public DataRetrieveCallback_base
        {
        // Public construction
        public:
            /// <summary>
            /// Constructor for callback structure.
            /// </summary>
            /// <param name="Ref">
            /// Reference to the class specified as the template argument.
            /// </param>
            /// <param name="Func">
            /// The member function, in the class specified as the template argument, to call in order to perform
            /// data retrieval operations.
            /// </param>
            DataRetrieveCallback_member(T* Ref, size_t (T::* Func)(void* const, const size_t)) :
                MemRef(Ref),
                MemFunc(Func)
            {
                
            }
            
        // Public member functions
        public:
            /// <summary>
            /// Overridden function from DataRetrieveCallback_base.
            /// Function to invoke in order to fill bufer with data.
        	/// Will invoke the member function of the class specified in the template argument.
            /// </summary>
            /// <param name="Buffer">
            /// A pointer to the buffer in which to store data.
            /// </param>
            /// <param name="MaxLen">
            /// The maximum amount of data to store into the buffer.
            /// </param>
        	/// <returns>
        	/// The number of bytes written into the buffer.
        	/// </returns>
            virtual size_t Invoke(void* const Buffer, const size_t MaxLen)
            {
                return (MemRef->*MemFunc)(Buffer, MaxLen);
            }
            
        // Private member variables
        private:
            /// <summary>
            /// A reference to the class specified in the template argument.
            /// </summary>
            T* MemRef;
            /// <summary>
            /// The member function in the class specified as the template argument.
            /// </summary>
            size_t (T::* MemFunc)(void* const Buffer, const size_t MaxLen);
        };

        /// <summary>
        /// Callback structure for data retrieval for input stream buffer, inheriting from DataRetrieveCallback_base,
        /// designed for specifying global functions as callbacks.
        /// </summary>
        struct DataRetrieveCallback_plain : public DataRetrieveCallback_base
        {
        // Public construction
        public:
            /// <summary>
            /// Constructor for callback structure.
            /// </summary>
            /// <param name="FuncRef">
            /// Pointer to a global function to call in order to perform data retrieval operations.
            /// </param>
            DataRetrieveCallback_plain(size_t (*FuncRef)(void* const, const size_t)) :
                Func(FuncRef)
            {
                
            }
            
        // Public member functions
        public:
            /// <summary>
            /// Overridden function from DataRetrieveCallback_base.
            /// Function to invoke in order to fill bufer with data.
        	/// Will invoke the global function specified in order to perform data retrieval operations.
            /// </summary>
            /// <param name="Buffer">
            /// A pointer to the buffer in which to store data.
            /// </param>
            /// <param name="MaxLen">
            /// The maximum amount of data to store into the buffer.
            /// </param>
        	/// <returns>
        	/// The number of bytes written into the buffer.
        	/// </returns>
            virtual size_t Invoke(void* const Buffer, const size_t MaxLen)
            {
                return (*Func)(Buffer, MaxLen);
            }
            
        // Private member variables
        private:
            /// <summary>
            /// Pointer to the global function to call in order to perform data retrieval operations.
            /// </summary>
            size_t (*Func)(void* const, const size_t);
        };
        
    // Public construction/destruction
    public:
        /// <summary>
        /// Constructor for the GenericIStreamBuf class.
        /// </summary>
        /// <param name="BufferSize">
        /// The size of the internal buffer.
        /// </param>
        GenericIStreamBuf(const size_t BufferSize);
        
    // Public member functions
    public:
        /// <summary>
        /// Will store the pointer an instance of the DataRetrieveCallback structure.
        /// </summary>
        /// <param name="DataRetCB">
        /// A pointer to an instance of the DataRetrieveCallback structure.
        /// </param>
        void RegisterOnDataRetrieveCallback(DataRetrieveCallback_base* const DataRetCB);
        
    // Protected member functions
    protected:
        /// <summary>
        /// Overridden function from GenericStreamBuf.
        /// The operation to perform upon underflow (lack of data to read) of data in internal buffer.
        /// Will require an instance of DataRetrieveCallback structure to be registered in order to retrieve
        /// additional data to be read.
        /// </summary>
        /// <returns></returns>
        virtual std::basic_streambuf<char>::int_type underflow();
        
    // Private member functions
    private:
        /// <summary>
        /// Will retrieve additional data for the streambuffer by utilizing the DataRetrieveCallback structure,
        /// if specified.
        /// </summary>
        void RetrieveData();
        
    // Private member variables
    private:
        /// <summary>
        /// A pointer to an instance of the callback structure responsible for retrieving additional data
        /// for the stream buffer.
        /// </summary>
        DataRetrieveCallback_base* DataRetrieveCB;
    };
}

#endif // GENERICISTREAMBUF_H
