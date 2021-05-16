#include <iostream>
#include <climits>
#include <math.h>
#include <assert.h>
#include <string>
#include <sstream>
#include <sys/mman.h>
#include <errno.h>
#include <numeric>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <time.h>

#include "src/bfsmpl.h"

void startCalculation()
{
    Integer A = Integer( 1, POSITIVE, 10000000 );
    Integer B = Integer( 1, POSITIVE, 10000000 );

    Integer::add( &A, &B );
}

#include "src/user/user.cpp"

int main(int argc, char *argv[])
{
    User::start();
}
