#include<iostream>
#include<csv_info.h>
#include<ml_utils.h>
#include<NDArray.cpp>

using namespace std;
int main()
{
csv_info csv=ml_utils::get_csv_info("i.csv",0);
NDArray x({csv.rows(),csv.columns()});
NDArray y({csv.rows()});
NDArray y_hat({csv.rows()});
NDArray difference({csv.rows()});
NDArray model({csv.columns()});		
NDArray u({csv.columns()});
NDArray difference_squared_sum({1});
NDArray::fill(model,{0},{csv_rows-1},0.23);
NDArray mse({1});
cout<<csv.rows()<<","<<csv.columns()<<endl;
ml_utils::get_x_y(csv,x,y);
NDArray x_transposed=x.as_transposed_view(true);
int n=csv.rows();
//x.print_for_testing();
//cout<<"-----------"<<endl;
//y.print_for_testing();
uint64_t number_of_iterations=100000;
uint64_t i;
double learning_rate=0.0005;
for(i=1;i<=number_of_iterations;++i)
{
	y_hat=x*model;
	difference=y_hat-y;
	difference_squared_sum=difference*difference;
	mse=difference_squared_sum/(double)(2*n);
	cout<<"Error : "<<mse[0]<<endl;
	u=x^T*difference;
	u/=(n);
	u*=learning_rate;
	model-=u;
}
return 0;
}
