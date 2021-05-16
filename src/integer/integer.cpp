#include "integer.h"

/*
 * Creates swap file too put integer in
 * */
void Integer::createSwap(uint64_t nBytes)
{
    // Generate strings needed to run the commands
    std::stringstream cmd, filePath;
    filePath << SWAP_DIRECTORY_LOCATION << SWAP_DIRECTORY << "/num-" << SWAP_ID;
    cmd << "dd if=/dev/zero of="  << SWAP_DIRECTORY_LOCATION << SWAP_DIRECTORY << "/" << "swap-" << SWAP_ID << " bs=1m count=" << 1 + safeMultiply( (uint64_t)8, nBytes ) / 1048576;

    // Attempt to create swap file
    std::cout << cmd.str().c_str() << std::endl;
    try {
        std::system(cmd.str().c_str());
    } catch(const std::exception& e)
    {
        std::cout << "Error: Swap file could not be created." << std::endl;
        std::exit(1);
    }

    // Increment the swap id
    SWAP_ID += 1;

    // Attempt to open the swap file
    std::cout << "Info: Swap file path: " << filePath.str().c_str() << std::endl;
    fd = open( filePath.str().c_str(), O_RDWR);
    if( fd == -1 ) {
        std::cout << "Error: Swap file could not be opened for mmap mapping." << std::endl;
        std::exit(1);
    }

    // Map the first MMAP_SIZE bytes into the address space
    mapSwapTooAddressSpace(0);
}

/*
 * Maps part of the swap file too the address space
 * */
void Integer::mapSwapTooAddressSpace( uint64_t _offset )
{
    assert( _offset < nDigits );

    // Make sure that a re-mapping is needed
    if( intMap == NULL || !(_offset >= offset/sizeof(DIGIT_TYPE) && _offset < offset/sizeof(DIGIT_TYPE) + MMAP_SIZE/sizeof(DIGIT_TYPE)) )
    {
        // Free the current mapping
        free();

        // Calculate closest offset in bits
        offset = _offset * sizeof(DIGIT_TYPE);
        uint64_t shift = offset % PAGE_SIZE;

        assert( shift % sizeof( DIGIT_TYPE ) == 0 );

        offset -= shift;

        // Map the file into address space
        intMap = (DIGIT_TYPE *)mmap(
            NULL,
            std::min((uint64_t)MMAP_SIZE, sizeof(DIGIT_TYPE)*nDigits - offset),
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            fd,
            offset
         );

        // Check that mapping worked
        if( intMap == MAP_FAILED )
        {
            std::cout << "Error: mapping file too address space failed. errno error code " << errno << std::endl;
            std::exit(1);
        }
    }
}

/*
 * Add A and B together. Final carry will be ignored
 */
void Integer::_add( Integer *A, Integer *B )
{
    DIGIT_TYPE N = std::max( A->nDigits, B->nDigits );
    DIGIT_TYPE M = (DIGIT_TYPE)-1;
    DIGIT_TYPE carry = 0;

    for( uint64_t i = 0; i < N; ++i )
    {
        DIGIT_TYPE a = A->getNthDigit(i);
        DIGIT_TYPE b = B->getNthDigit(i);

        A->setNthDigit(i, a+b+carry);
        carry = ( a + carry < M - b ) ? 0 : 1;
    }
}

/*
 * Makes sure, if ERR_CHECKING is true, that there is no overflow. Returns a*b
 * */
template<typename T>
T Integer::safeMultiply( T a, T b)
{
    T res = a * b;
    assert( res / b == a );
    return res;
}

/*
 * Constructor that handles the least significant digit as the input
 */
Integer::Integer( DIGIT_TYPE integer_, bool sign_, uint64_t nDigits_)
{
    assert( nDigits_ > 0 );

    nDigits = nDigits_;
    sign = sign_;

    createSwap(nDigits_*sizeof( DIGIT_TYPE ));
    // setNthDigit(0, integer_);
}

/*
 * Class destructor
 * */
Integer::~Integer()
{
    free();
}

/*
 * Function that handles the addition
 */
void Integer::add( Integer *A, Integer *B )
{
    Integer::_add(A, B);
}

/*
 * Returns the number of digits in the integer
 * */
uint64_t Integer::getNDigits()
{
    return nDigits;
}

/*
 * Gets the nth digit of the integer
 * */
DIGIT_TYPE Integer::getNthDigit(uint64_t n)
{
    if( n < nDigits )
    {
        mapSwapTooAddressSpace( n );
        return intMap[n - offset/sizeof(DIGIT_TYPE)];
    } else
    {
        return 0;
    }
}

/*
 * Sets the nth digit of the integer at too v
 * */
void Integer::setNthDigit(uint64_t n, DIGIT_TYPE v)
{
    if(n < nDigits) {
        mapSwapTooAddressSpace( n );
        intMap[ n - offset/sizeof(DIGIT_TYPE) ] = v;
    }
}

/*
 * Frees the integer array
 * */
void Integer::free()
{
    if( munmap( intMap, MMAP_SIZE ) )
    {
        std::cout << "Error: unmapping file too address space failed" << std::endl;
        std::exit(1);
    }
}

/*
 * Prints the integer, with its sign, in big endian order where each digit is a base 16
 * positive number,
 * */
void Integer::print()
{
    assert( nDigits > 0 );

    if( !sign ) std::cout << "- ";
    else std::cout << "  ";

    for( uint64_t i = nDigits; i > 0; --i )
    {
        std::cout << std::hex << (uint64_t) getNthDigit(i - 1) << " ";
    }

    std::cout << std::endl;
}
