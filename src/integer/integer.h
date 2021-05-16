/*
 * Integer is a class for storing and manipulating multi-precision integers.
 * It uses 64-bit integer arrays to store integers. All other classes are derived
 * from this class and build on top of it's functions. It is designed for pure speed
 * and not necessarily convenience.
 * */

class Integer
{
    private:
        /* portion of the integer that is mapped into memory */
        DIGIT_TYPE *intMap = NULL;

        /* The offset of the mapped file */
        uint64_t offset = 0;

        /* Some integer representing the path to a file */
        int fd;

        /* The size of the integer in bytes */
        uint64_t nDigits;

        /* Integer memory management */
        void createSwap( uint64_t nBytes );
        void mapSwapTooAddressSpace( uint64_t _offset );

        /*Multiplication Functions*/

        /* Addition and subtraction functions */
        static void _add( Integer *A, Integer *B );

        /* Sage operations. Will check that operation was performed correctly */
        template<class T>
        T safeMultiply( T a, T b );

        template<class T>
        T safeAdd( T a, T b );

        template<class T>
        T safeDivide( T a, T b );

        template<class T>
        T safeSubtract( T a, T b );

    public:
        /* The sign of the integer. 1 implies positive, 0 implies negative */
        bool sign;

        /* The constructors and destructors for this class */
        Integer() {}
        Integer( DIGIT_TYPE integer_, bool sign_, uint64_t nDigits_);
        ~Integer();

        /* Read/write to the integer */
        DIGIT_TYPE getNthDigit( uint64_t n );
        void setNthDigit( uint64_t n, DIGIT_TYPE v );

        /* Operation */
        static void add( Integer *A, Integer *B );

        /* Helper methods */
        uint64_t getNDigits();
        void free();
        void print();
};
