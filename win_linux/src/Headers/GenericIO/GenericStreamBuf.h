#ifndef GENERICSTREAMBUF_H
#define GENERICSTREAMBUF_H

#include <streambuf>

namespace GenericIO
{
    class GenericStreamBuf : public std::basic_streambuf<char>
    {
    // Public construction/destruction
    public:
    	GenericStreamBuf(void* const Mem, const size_t BufferSize);
        GenericStreamBuf(const size_t BufferSize);
        ~GenericStreamBuf();
        
    // Public member functions
    public:
        void PrintStreamBuf() const;
        void PrintStatus() const;
        
    // Protected member functions
    protected:
        virtual std::basic_streambuf<char>::int_type overflow(std::basic_streambuf<char>::int_type ch);
        virtual std::basic_streambuf<char>::int_type underflow();
        virtual std::basic_streambuf<char>::pos_type seekoff(std::basic_streambuf<char>::off_type off, std::ios_base::seekdir dir,
                            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
        virtual std::basic_streambuf<char>::pos_type seekpos(std::basic_streambuf<char>::pos_type pos,
                            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
        virtual std::streamsize showmanyc();
        
    // Private member functions    
    private:
        std::basic_streambuf<char>::pos_type seekoff_in(const std::basic_streambuf<char>::off_type off, const std::ios_base::seekdir dir);
        std::basic_streambuf<char>::pos_type seekoff_out(const std::basic_streambuf<char>::off_type off, const std::ios_base::seekdir dir);
        std::basic_streambuf<char>::off_type DetermineMovement(void* const Current, const std::basic_streambuf<char>::off_type Offset,
                            void* const Max, void* const Min) const;
        
        
    // Private member variables
    private:
        const bool IsPredefinedMemory;
        const size_t MemLength;
        char* InternalMem;
        bool PCAdvance;
    };
}

#endif // GENERICSTREAMBUF_H
