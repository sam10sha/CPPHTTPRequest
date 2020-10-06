#ifndef GENERICSTREAMBUF_H
#define GENERICSTREAMBUF_H

#include <streambuf>

namespace GenericIO
{
	/// <summary>
	/// Stream buffer with internal circular buffer for IO operations.
	/// </summary>
    class GenericStreamBuf : public std::basic_streambuf<char>
    {
    // Public construction/destruction
    public:
    	/// <summary>
    	/// Constructor using predefined memory for internal buffer.
    	/// </summary>
    	/// <param name="Mem">
    	/// Predefined memory to use for the internal buffer.
    	/// </param>
    	/// <param name="BufferSize">
    	/// Size of predefined memory.
    	/// </param>
    	GenericStreamBuf(void* const Mem, const size_t BufferSize);
    	/// <summary>
    	/// Constructor having memory for internal buffer allocated during construction.
    	/// </summary>
    	/// <param name="BufferSize">
    	/// Size of memory to be allocated.
    	/// </param>
        GenericStreamBuf(const size_t BufferSize);
    	/// <summary>
    	/// Destructor which will destroy memory only if allocated during construction of object.
    	/// </summary>
        ~GenericStreamBuf();
        
    // Protected member functions
    protected:
    	/// <summary>
    	/// Overridden function from basic_streambuf.
    	/// The operation to perform upon overflow (lack of space to write) of data in internal buffer.
    	/// If previously written data has been read, will extend put area to accommodate this memory space and allow
    	/// previously written data to be overwritten.
    	/// </summary>
    	/// <param name="ch">
    	/// The character to write to the internal buffer.
    	/// </param>
    	/// <returns>
    	/// Upon successful write, the character is returned as integer type.
    	/// EOF is returned if no further data can be written.
    	/// </returns>
        virtual std::basic_streambuf<char>::int_type overflow(std::basic_streambuf<char>::int_type ch);
    	/// <summary>
    	/// Overridden function from basic_streambuf.
    	/// The operation to perform upon underflow (lack of data to read) of data in internal buffer.
    	/// If unread data exists, will extend get area to accommodate the additional memory space and allow unread
    	/// data to be read.
    	/// </summary>
    	/// <returns>
    	/// Upon successful read, the latest unread character will be returned as integer type.
    	/// EOF is returned if no further data can be read.
    	/// </returns>
        virtual std::basic_streambuf<char>::int_type underflow();
    	/// <summary>
    	/// Will adjust the position of a pointer in the put area or get area.
    	/// </summary>
    	/// <param name="off">
    	/// The number of elements by which to shift the selected pointer.
    	/// </param>
    	/// <param name="dir">
    	/// The selector determining which specific pointer in put area or get area to modify.
    	/// Options are: BEG, CUR, END.
    	/// </param>
    	/// <param name="which">
    	/// The selector determining which set of pointers to modify.
    	/// IN specifies the get pointers. OUT specifies the put pointers.
    	/// </param>
    	/// <returns>
    	/// The new location of the selected pointer.
    	/// </returns>
        virtual std::basic_streambuf<char>::pos_type seekoff(std::basic_streambuf<char>::off_type off, std::ios_base::seekdir dir,
                            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    	/// <summary>
    	/// Not implemented. See superclass implementation.
    	/// </summary>
    	/// <param name="pos"></param>
    	/// <param name="which"></param>
    	/// <returns></returns>
        virtual std::basic_streambuf<char>::pos_type seekpos(std::basic_streambuf<char>::pos_type pos,
                            std::ios_base::openmode which = std::ios_base::in | std::ios_base::out);
    	/// <summary>
    	/// Will determine the number of characters left to read in internal buffer.
    	/// </summary>
    	/// <returns>
    	/// The number of bytes available to read.
    	/// </returns>
        virtual std::streamsize showmanyc();
        
    // Private member functions    
    private:
    	/// <summary>
    	/// Will move the get pointers by the desired number of elements.
    	/// </summary>
    	/// <param name="off">
        /// The desired number of elements by which to move the selected pointer.
        /// </param>
    	/// <param name="dir">
    	/// The selector determining which pointer in the get area to modify.
        /// Options are: BEG, CUR, END.
    	/// </param>
    	/// <returns>
        /// The new position of the pointer.
        /// </returns>
        std::basic_streambuf<char>::pos_type seekoff_in(const std::basic_streambuf<char>::off_type off, const std::ios_base::seekdir dir);
    	/// <summary>
    	/// Will move the put pointers by the desired number of elements.
    	/// </summary>
    	/// <param name="off">
        /// The desired number of elements by which to move the selected pointer.
        /// </param>
    	/// <param name="dir">
    	/// The selector determining which pointer in the put area to modify.
        /// Options are: BEG, CUR, END.
    	/// </param>
    	/// <returns>
        /// The new position of the pointer.
        /// </returns>
        std::basic_streambuf<char>::pos_type seekoff_out(const std::basic_streambuf<char>::off_type off, const std::ios_base::seekdir dir);
    	/// <summary>
    	/// Helper class for seekoff_in and seekoff_out. Will determine the amount of allowable movement for a selected
    	/// pointer in the put area or get area.
    	/// </summary>
    	/// <param name="Current">
    	/// The current location of the pointer.
    	/// </param>
    	/// <param name="Offset">
    	/// The desired amount of elements by which to move the pointer.
    	/// </param>
    	/// <param name="Max">
    	/// The upper limit of the range of positions at which the pointer can be located.
    	/// </param>
    	/// <param name="Min">
    	/// The lower limit of the range of positions at which the pointer can be located.
    	/// </param>
    	/// <returns>
    	/// The number of elements by which the pointer will move.
    	/// </returns>
        std::basic_streambuf<char>::off_type DetermineMovement(void* const Current, const std::basic_streambuf<char>::off_type Offset,
                            void* const Max, void* const Min) const;
        
        
    // Private member variables
    private:
    	/// <summary>
    	/// True if memory is defined externally.
    	/// False if memory is allocated during construction.
    	/// </summary>
        const bool IsPredefinedMemory;
    	/// <summary>
    	/// The size of the internal memory.
    	/// </summary>
        const size_t MemLength;
    	/// <summary>
    	/// Pointer to location of internal memory.
    	/// </summary>
        char* InternalMem;
    	/// <summary>
    	/// PutCounterAdvance.
    	/// True if put pointer is ahead of get pointer.
    	/// False if put pointer is even with get pointer.
    	/// </summary>
        bool PCAdvance;
    };
}

#endif // GENERICSTREAMBUF_H
