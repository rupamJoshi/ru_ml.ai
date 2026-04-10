#include<NDArray.h>
#include<NDArrayTransposedView.h>
#include<iostream>
#include<iomanip>
using namespace std;
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
			cout<<setw(10)<<n[r][c]; 
		}
		cout<<endl;
	}
}
int main()
{
	NDArray n({4,3});
	uint32_t r,c;
	double value=1.0;
	for(r=0;r<4;++r)
	{
		for(c=0;c<3;++c)
		{
		 n[r][c]=value++;
		}
	}
	cout<<"n matrix"<<endl;
	print_2d(n);

	NDArrayTransposedView nt=n.as_transposed_view(true);
	print_2d(nt);
	return 0;
}
