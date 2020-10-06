#include "GenericStreamBuf.h"
#include "stdafx.h"

// Public construction/destruction
GenericIO::GenericStreamBuf::GenericStreamBuf(void* const Mem, const size_t BufferSize) :
	IsPredefinedMemory(true),
	MemLength(BufferSize),
	InternalMem((char*)Mem),
	PCAdvance(false)
{
    setp(InternalMem, InternalMem);
    setg(InternalMem, InternalMem, InternalMem);
}
GenericIO::GenericStreamBuf::GenericStreamBuf(const size_t BufferSize) :
	IsPredefinedMemory(false),
    MemLength(BufferSize),
    InternalMem(new char[BufferSize]),
    PCAdvance(false)
{
    std::memset(InternalMem, 0, BufferSize);
    setp(InternalMem, InternalMem);
    setg(InternalMem, InternalMem, InternalMem);
}

GenericIO::GenericStreamBuf::~GenericStreamBuf()
{
    if(!IsPredefinedMemory && InternalMem)
    {
        delete[] InternalMem;
        InternalMem = nullptr;
    }
}


// Protected member functions
std::basic_streambuf<char>::int_type GenericIO::GenericStreamBuf::overflow(std::basic_streambuf<char>::int_type ch)
{
    std::basic_streambuf<char>::int_type Result;
    
    //std::basic_streambuf<char>::char_type* PutBegin = pbase();
    std::basic_streambuf<char>::char_type* PutEnd = epptr();
    //std::basic_streambuf<char>::char_type* PutCurrent = pptr();
    
    //std::basic_streambuf<char>::char_type* GetBegin = eback();
    std::basic_streambuf<char>::char_type* GetEnd = egptr();
    std::basic_streambuf<char>::char_type* GetCurrent = gptr();
    
    if(PutEnd > GetEnd)
    {
        if(PutEnd < InternalMem + MemLength)
        {
            setp(PutEnd, InternalMem + MemLength);
        }
        else
        {
            setp(InternalMem, GetCurrent);
        }
    }
    else if(PutEnd < GetEnd)
    {
        setp(PutEnd, GetCurrent);
    }
    else if(!PCAdvance)
    {
        if(PutEnd < InternalMem + MemLength)
        {
            setp(PutEnd, InternalMem + MemLength);
        }
        else
        {
            setp(InternalMem, GetCurrent);
        }
    }
	
    PCAdvance = true;
    if(epptr() - pptr() > 0)
    {
        Result = sputc(ch);
    }
    else
    {
        Result = std::basic_streambuf<char>::overflow(ch);
    }
    
    return Result;
}
std::basic_streambuf<char>::int_type GenericIO::GenericStreamBuf::underflow()
{
    std::basic_streambuf<char>::int_type Result;
    
    //std::basic_streambuf<char>::char_type* PutBegin = pbase();
    std::basic_streambuf<char>::char_type* PutEnd = epptr();
    std::basic_streambuf<char>::char_type* PutCurrent = pptr();
    
    //std::basic_streambuf<char>::char_type* GetBegin = eback();
    std::basic_streambuf<char>::char_type* GetEnd = egptr();
    //std::basic_streambuf<char>::char_type* GetCurrent = gptr();
    
    if(GetEnd > PutEnd)
    {
        if(GetEnd < InternalMem + MemLength)
        {
            setg(GetEnd, GetEnd, InternalMem + MemLength);
        }
        else
        {
            setg(InternalMem, InternalMem, PutCurrent);
        }
    }
    else if(GetEnd < PutEnd)
    {
        setg(GetEnd, GetEnd, PutCurrent);
    }
    else if(PCAdvance)
    {
        if(GetEnd < InternalMem + MemLength)
        {
            setg(GetEnd, GetEnd, InternalMem + MemLength);
        }
        else
        {
            setg(InternalMem, InternalMem, PutCurrent);
        }
    }
    
    PCAdvance = PutEnd != egptr();
    if(egptr() - gptr() > 0)
    {
        Result = sgetc();
    }
    else
    {
        Result = std::basic_streambuf<char>::underflow();
    }
    return Result;
}
std::basic_streambuf<char>::pos_type GenericIO::GenericStreamBuf::seekoff(std::basic_streambuf<char>::off_type off, std::ios_base::seekdir dir,
                            std::ios_base::openmode which)
{
    std::basic_streambuf<char>::pos_type ResultPos;
    switch(which)
    {
        case std::ios_base::in:
            ResultPos = seekoff_in(off, dir);
            break;
        case std::ios_base::out:
            ResultPos = seekoff_out(off, dir);
            break;
        default:
            ResultPos = std::basic_streambuf<char>::seekoff(off, dir, which);
            break;
    }
    return ResultPos;
}
std::basic_streambuf<char>::pos_type GenericIO::GenericStreamBuf::seekpos(std::basic_streambuf<char>::pos_type pos,
                            std::ios_base::openmode which)
{
    return std::basic_streambuf<char>::seekpos(pos, which);
}
std::streamsize GenericIO::GenericStreamBuf::showmanyc()
{
    std::streamsize NumChars = 0;
    //std::basic_streambuf<char>::char_type* PutBegin = pbase();
    std::basic_streambuf<char>::char_type* PutEnd = epptr();
    std::basic_streambuf<char>::char_type* PutCurrent = pptr();
    
    //std::basic_streambuf<char>::char_type* GetBegin = eback();
    std::basic_streambuf<char>::char_type* GetEnd = egptr();
    std::basic_streambuf<char>::char_type* GetCurrent = gptr();
    
    if(GetEnd > PutEnd)
    {
        // ((InternalMem + MemLength) - GetCurrent) + (PutCurrent - InternalMem)
        NumChars = PutCurrent + MemLength - GetCurrent;
    }
    else if(GetEnd < PutEnd)
    {
        NumChars = PutCurrent - GetCurrent;
    }
    else if(PCAdvance)
    {
        // ((InternalMem + MemLength) - GetCurrent) + (PutCurrent - InternalMem)
        NumChars = PutCurrent + MemLength - GetCurrent;
    }
    
    return NumChars;
}

// Private member functions
std::basic_streambuf<char>::pos_type GenericIO::GenericStreamBuf::seekoff_in(const std::basic_streambuf<char>::off_type off,
                            const std::ios_base::seekdir dir)
{
    std::basic_streambuf<char>::pos_type ResultPos;
    std::basic_streambuf<char>::char_type* LimitPtr;
    switch(dir)
    {
        case std::ios_base::beg:
            LimitPtr = gptr();
            setg(eback() + DetermineMovement(eback(), off, LimitPtr, InternalMem), LimitPtr, epptr());
            ResultPos = std::basic_streambuf<char>::pos_type(eback() - InternalMem);
            break;
        case std::ios_base::cur:
            gbump(DetermineMovement(gptr(), off, egptr(), eback()));
            ResultPos = std::basic_streambuf<char>::pos_type(gptr() - InternalMem);
            break;
        /* case std::ios_base::end:
            LimitPtr = InternalMem + MemLength;
            setg(pbase(), LimitPtr, egptr() + DetermineMovement(egptr(), off, LimitPtr, gptr()));
            ResultPos = std::basic_streambuf<char>::pos_type(egptr() - InternalMem);
            break; */
        default:
            ResultPos = std::basic_streambuf<char>::pos_type(0);
            break;
    }
    return ResultPos;
}
std::basic_streambuf<char>::pos_type GenericIO::GenericStreamBuf::seekoff_out(const std::basic_streambuf<char>::off_type off,
                            const std::ios_base::seekdir dir)
{
    std::basic_streambuf<char>::pos_type ResultPos;
    std::basic_streambuf<char>::char_type* LimitPtr;
    switch(dir)
    {
        case std::ios_base::beg:
            LimitPtr = pptr();
            setp(pbase() + DetermineMovement(pbase(), off, LimitPtr, InternalMem), epptr());
            pbump(LimitPtr - pbase());
            ResultPos = std::basic_streambuf<char>::pos_type(pbase() - InternalMem);
            break;
        case std::ios_base::cur:
            pbump(DetermineMovement(pptr(), off, epptr(), pbase()));
            ResultPos = std::basic_streambuf<char>::pos_type(pptr() - InternalMem);
            break;
        /* case std::ios_base::end:
            LimitPtr = pptr();
            setp(pbase(), epptr() + DetermineMovement(epptr(), off, InternalMem + MemLength, LimitPtr));
            pbump(LimitPtr - pbase());
            ResultPos = std::basic_streambuf<char>::pos_type(epptr() - InternalMem);
            break; */
        default:
            ResultPos = std::basic_streambuf<char>::pos_type(0);
            break;
    }
    return ResultPos;
}
std::basic_streambuf<char>::off_type GenericIO::GenericStreamBuf::DetermineMovement(void* const Current, const std::basic_streambuf<char>::off_type Offset,
                            void* const Max, void* const Min) const
{
    void* NewPos;
    std::basic_streambuf<char>::off_type DeterminedOffset;
    NewPos = (char*)Current + Offset;
    if(NewPos >= Min && NewPos < Max)
    {
        DeterminedOffset = Offset;
    }
    else
    {
        DeterminedOffset = (Offset >= 0) ? (char*)Max - (char*)Current : (char*)Current - (char*)Min;
    }
    return DeterminedOffset;
}

