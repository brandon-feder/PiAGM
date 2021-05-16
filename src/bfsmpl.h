// For choosing the sign of an integer
#define NEGATIVE 0
#define POSITIVE 1

// For comparisons
#define GREATER 1
#define EQUAL 0
#define LESS -1

// Type of the digit being stored. Most be unsigned
#define DIGIT_TYPE uint64_t

// Whether to remove previous swap fiel directories
#define REMOVE_PREVIOUS_SWAPS true

// Name of swap directory
#define SWAP_DIRECTORY "swap"

// Path too swap file directory
#define SWAP_DIRECTORY_LOCATION "/Users/brandonf/Documents/BrandonMPL/"

// The size of the memory page
uint64_t PAGE_SIZE = sysconf(_SC_PAGE_SIZE);

// The size of the map that is used
uint64_t MMAP_SIZE = std::lcm( PAGE_SIZE, sizeof(DIGIT_TYPE) );

uint64_t SWAP_ID = 0; // Id of the first created swap file. This id will be iterated for the next swap file

#include "integer/integer.cpp"
#include "test/test.cpp"
