#ifndef GENERICISTREAMBUF_H
#define GENERICISTREAMBUF_H

#include "GenericStreamBuf.h"
#include <streambuf>

namespace GenericIO
{
    class GenericIStreamBuf : public GenericStreamBuf
    {
    // Public structures
    public:
        struct DataRetrieveCallback_base
        {
        // Public member functions
        public:
            /// <summary>
            /// 
            /// </summary>
            /// <param name="Buffer"></param>
            /// <param name="MaxLen"></param>
            virtual size_t Invoke(void* const Buffer, const size_t MaxLen)
            {
                return 0;
            }
        };
        
        template<class T>
        struct DataRetrieveCallback_member : public DataRetrieveCallback_base
        {
        // Public construction
        public:
            /// <summary>
            /// 
            /// </summary>
            /// <param name="Ref"></param>
            /// <param name="Func"></param>
            DataRetrieveCallback_member(T* Ref, size_t (T::* Func)(void* const, const size_t)) :
                MemRef(Ref),
                MemFunc(Func)
            {
                
            }
            
        // Public member functions
        public:
            /// <summary>
            /// 
            /// </summary>
            /// <param name="Buffer"></param>
            /// <param name="MaxLen"></param>
            virtual size_t Invoke(void* const Buffer, const size_t MaxLen)
            {
                return (MemRef->*MemFunc)(Buffer, MaxLen);
            }
            
        // Private member variables
        private:
            T* MemRef;
            size_t (T::* MemFunc)(void* const Buffer, const size_t MaxLen);
        };
        
        struct DataRetrieveCallback_plain : public DataRetrieveCallback_base
        {
        // Public construction
        public:
            /// <summary>
            /// 
            /// </summary>
            /// <param name="FuncRef"></param>
            DataRetrieveCallback_plain(size_t (*FuncRef)(void* const, const size_t)) :
                Func(FuncRef)
            {
                
            }
            
        // Public member functions
        public:
            /// <summary>
            /// 
            /// </summary>
            /// <param name="Buffer"></param>
            /// <param name="MaxLen"></param>
            virtual size_t Invoke(void* const Buffer, const size_t MaxLen)
            {
                return (*Func)(Buffer, MaxLen);
            }
            
        // Private member variables
        private:
            size_t (*Func)(void* const, const size_t);
        };
        
    // Public construction/destruction
    public:
        GenericIStreamBuf(const size_t BufferSize);
        
    // Public member functions
    public:
        void RegisterOnDataRetrieveCallback(DataRetrieveCallback_base* const DataRetCB);
        
    // Protected member functions
    protected:
        virtual std::basic_streambuf<char>::int_type underflow();
        
    // Private member functions
    private:
        void RetrieveData();
        
    // Private member variables
    private:
        DataRetrieveCallback_base* DataRetrieveCB;
    };
}

#endif // GENERICISTREAMBUF_H
