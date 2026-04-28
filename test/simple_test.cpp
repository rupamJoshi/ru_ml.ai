#include <NDArray.h>
#include <NDArrayTransposedView.h>
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    NDArray n({2, 2, 1});

    n[0][0][1] = 1;

    n.print_for_testing();

    return 0;
}