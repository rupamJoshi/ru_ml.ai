#include<NDArray.h>
#include<NDArrayTransposedView.h>
#include<iostream>
#include<iomanip>
using namespace std;
#define TARGET_VALUE 2000
template<typename T>
void print_2d(T &n)
{
	auto dimensions=n.get_dimensions();
	if(dimensions.size()==1) 
	{
		cout<<"Cannot be printed as 2d matrix"<<endl;
		return;
	}
	uint32_t rows=dimensions[0];
	uint32_t columns=dimensions[1];
	for(uint32_t r=0;r<rows;++r)
	{
		for(uint32_t c=0;c<columns;++c)
		{
			cout<<"[" << n.get(r,c)<<"]"<< " "; 
		}
		cout<<endl;
	}
}
int main()
{
	NDArray n({TARGET_VALUE,TARGET_VALUE});
	uint32_t r,c;
	double value=1.0;
	for(r=0;r<TARGET_VALUE;++r)
	{
		for(c=0;c<TARGET_VALUE;++c)
		{
		 n[r][c]=value++;
		}
	}
	NDArray n1({TARGET_VALUE,TARGET_VALUE});

	for(r=0;r<TARGET_VALUE;++r)
	{
		for(c=0;c<TARGET_VALUE;++c)
		{
		 n[r][c]=value++;
		}
	}
	NDArray n3({TARGET_VALUE,TARGET_VALUE});
	n3 = n1 * n;
	// cout<<"n matrix"<<endl;
	// print_2d(n);
	// NDArrayTransposedView nt=n.as_transposed_view(true);
	// cout<<"nt matrix"<<endl;
	// print_2d(nt);

	// NDArray productMatrix({3, 3});
	// productMatrix = nt*n;
	// print_2d(productMatrix);
	return 0;
}
