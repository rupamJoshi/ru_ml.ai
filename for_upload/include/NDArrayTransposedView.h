#pragma once
#include<NDArray.h>
using namespace std;
class NDArray; // important forward declaration
class NDArrayTransposedView
{
	private:
		NDArray *ndArray;
		bool is_read_only;
	public:
		NDArrayTransposedView(NDArray *ndArray,bool is_read_only);
};
