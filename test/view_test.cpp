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
	cout << dimensions.size() << endl;
	uint32_t rows = dimensions[0];
	uint32_t columns = dimensions[1];
	cout << rows << " and " << columns << endl;
	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			cout << "[" << n.get(r, c) << "]" << " ";
		}
		cout << endl;
	}
}
template <typename T>
void print_1d(T &n)
{

	cout << "Printing one d array ========= " << endl;
	auto dimensions = n.get_dimensions();
	uint32_t rows = dimensions[0];
	for (uint32_t r = 0; r < rows; ++r)
	{
		cout << "[" << n.get(r) << "]" << " r  : " << r << endl;
		cout << endl;
	}
}

template <typename T>
void print_3d(T &n)
{
	auto dimensions = n.get_dimensions();
	cout << dimensions.size() << endl;
	uint32_t rows = dimensions[0];
	cout << "Rows : " << rows << endl;
	for (uint32_t r = 0; r < rows; ++r)
	{
		cout << "[=" << n.get(r) << "]" << " r  : " << r;
		cout << endl;
	}
}

template <typename T>
void print_4d(T &n)
{
	auto dimensions = n.get_dimensions();
	cout << dimensions.size() << endl;
	uint32_t rows = dimensions[0];
	cout << "Rows : " << rows << endl;
	for (uint32_t r = 0; r < rows; ++r)
	{
		cout << "[=" << n.get(r) << "]" << " r  : " << r;
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
	// print_2d(n);
	vector<uint32_t> from = {1, 5};
	vector<uint32_t> to = {5, 5};

	NDArrayView view = n.as_view(from, to);
	print_1d(view);

	NDArray n1({TARGET_VALUE, TARGET_VALUE});

	vector<uint32_t> from1 = {0, 1};
	vector<uint32_t> to1 = {0, 5};

	NDArrayView view1 = n1.as_view(from1, to1);
	print_2d(view1);
	cout << "----------------------------" << endl;

	NDArray n2({3, 6, 6});
	value = 1.0;
	for (int i = 0; i < 3; ++i)
	{
		for (r = 0; r < TARGET_VALUE; ++r)
		{
			for (c = 0; c < TARGET_VALUE; ++c)
			{
				n2[i][r][c] = value++;
			}
		}
	}

	vector<uint32_t> from2 = {2, 6, 2};
	vector<uint32_t> to2 = {3, 6, 2};

	NDArrayView view2 = n2.as_view(from2, to2);

	NDArray n3({3, 3});
	value = 1.0;
	for (r = 0; r < 3; ++r)
	{
		for (c = 0; c < 3; ++c)
		{
			n3[r][c] = value++;
		}
	}

	vector<uint32_t> from3 = {3, 1};
	vector<uint32_t> to3 = {3, 2};

	NDArrayView view3 = n3.as_view(from3, to3);

	NDArray n4({2, 3, 3, 3});
	value = 1.0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (r = 0; r < 3; ++r)
			{
				for (c = 0; c < 3; ++c)
				{
					n4[i][j][r][c] = value++;
				}
			}
		}
	}

	vector<uint32_t> from4 = {2, 3, 3, 1};
	vector<uint32_t> to4 = {2, 3, 3, 2};

	NDArrayView view4 = n4.as_view(from4, to4);

	NDArray n5({2, 3, 3, 3});
	value = 1.0;
	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (r = 0; r < 3; ++r)
			{
				for (c = 0; c < 3; ++c)
				{
					n5[i][j][r][c] = value++;
				}
			}
		}
	}

	vector<uint32_t> from5 = {1, 2, 3, 1};
	vector<uint32_t> to5 = {2, 3, 3, 2};

	NDArrayView view5 = n5.as_view(from5, to5);

	NDArray n6({TARGET_VALUE, TARGET_VALUE});

	vector<uint32_t> from6 = {0, 1};
	vector<uint32_t> to6 = {1, 5};

	NDArrayView view6 = n6.as_view(from6, to6);
	// print_1d(view1);

	NDArray n7({TARGET_VALUE, TARGET_VALUE});

	vector<uint32_t> from7 = {1, 1};
	vector<uint32_t> to7 = {1, 1};

	NDArrayView view7 = n7.as_view(from7, to7);
	// print_1d(view1);

	return 0;
}
