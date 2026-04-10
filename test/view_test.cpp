#include <NDArray.h>
#include <NDArrayTransposedView.h>
#include <iostream>
#include <NDArrayView.h>
#include <iomanip>
using namespace std;
#define TARGET_VALUE 6
template <typename T>
void print_2d(T &n)
{
	auto dimensions = n.get_dimensions();
	if (dimensions.size() == 1)
	{
		cout << "Cannot be printed as 2d matrix" << endl;
		return;
	}
	uint32_t rows = dimensions[0];
	uint32_t columns = dimensions[1];
	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			cout << "[" << n.get(r, c) << "]" << " ";
		}
		cout << endl;
	}
}
int main()
{
	NDArray n({TARGET_VALUE, TARGET_VALUE});
	uint32_t r, c;
	double value = 1.0;
	for (r = 0; r < TARGET_VALUE; ++r)
	{
		for (c = 0; c < TARGET_VALUE; ++c)
		{
			n[r][c] = value++;
		}
	}
	vector<uint32_t> from = {0, 2};
	vector<uint32_t> to = {0, 5};

	NDArrayView view = n.as_view(from, to);
	print_2d(view);

	return 0;
}
