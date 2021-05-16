#include "user.h"

void User::check()
{
    assert( MMAP_SIZE % PAGE_SIZE == 0 );
    assert( MMAP_SIZE > 0 );
}

void User::removeSwapDirectory()
{
    std::stringstream rmCmd;
    rmCmd << "rm -R " << SWAP_DIRECTORY_LOCATION << SWAP_DIRECTORY;

    std::cout << rmCmd.str().c_str() << std::endl;
    if(std::system(rmCmd.str().c_str()))
    {
        std::cout << "Info: Can not find swap directory. Nothing was deleted." << std::endl;
    }
}

void User::createSwapDirectory()
{
    std::stringstream mkdirCmd;
    mkdirCmd << "mkdir " << SWAP_DIRECTORY_LOCATION << SWAP_DIRECTORY;

    std::cout << mkdirCmd.str().c_str() << std::endl;
    if(std::system(mkdirCmd.str().c_str()))
    {
        std::cout << "Error: Could not create swap directory." << std::endl;
        std::exit(1);
    }
}

void User::start()
{
    std::string cmd;
    while(true)
    {
        std::cout << "-->";
        std::cin >> cmd;

        if( cmd == "clear-swap")
        {
            removeSwapDirectory();
            createSwapDirectory();
        } else if( cmd == "start" )
        {
            startCalculation();
        }
        else if( cmd == "quit")
        {
            std::string sure;
            std::cout << "Are you sure? (Y/N)";
            std::cin >> sure;
            if( sure == "Y" ) return;
        } else if( cmd == "set" )
        {
            std::string param1;
            std::cin >> param1;

            if( param1 == "mmap-size-factor")
            {
                std::string param2;
                std::cin >> param2;
                assert( std::stoll(param2) > 0 );
                MMAP_SIZE = std::lcm( PAGE_SIZE, sizeof(DIGIT_TYPE) ) * std::stoll( param2 );
            } else
            {
                std::cout << "Error: Invalid parameter for set" << std::endl;
            }
        } else if( cmd == "get" )
        {
            std::string param1;
            std::cin >> param1;

            if( param1 == "mmap-size" )
            {
                std::cout << "MMAP_SIZE: " << MMAP_SIZE << " bytes" << std::endl;
            } else
            {
                std::cout << "Error: Invalid parameter for get" << std::endl;
            }
        } else
        {
            std::cout << "Error: Invalid command" << std::endl;
        }
    }
}
