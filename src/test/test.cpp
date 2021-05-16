/*
 * Prints A and B. Then, performs the passed function on A and B and prints the result.
 * */

template< class T >
void test( T A, T B, void ( *operation )( T, T ) )
{
    A.print();
    B.print();
    operation(A, B);
    A.print();
}
