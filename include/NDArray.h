#pragma once
#include<thread>
#include<iostream>
#include<vector>
#include<exception>
#include<numeric>
#include<stdint.h>
#include<unordered_set>
#include<sstream>
#include <InfixExpression.h>

#include<NDArrayNode.h>
//#include<NDArrayTransposedView.h>
#include<NDArrayException.h>
#define T1 double
using namespace std;
class NDArrayNode;
class NDArrayTransposedView;
class NDArrayView;
class NDArray
{
	private:

		std::vector<T1> collection;
		std::vector<uint32_t> dimensions;
		std::unordered_set<NDArrayNode *> nodes; 
		std::vector<T1>& get_raw_collection();
		std::vector<uint32_t>& get_raw_dimensions();


		void _set(uint32_t,T1);
		T1 _get(uint32_t);
		NDArray();
	public:
		NDArray(const std::vector<uint32_t> &);
		NDArray(const NDArray &);
		NDArray(NDArray &&);
		NDArray & operator=(const NDArray &);
		NDArray & operator=(NDArray &&);
		virtual ~NDArray();
		
		template<typename... TT>
		void set(TT ...);
		
		template<typename... TT>
		T1 get(TT ...);

		template<typename TT1,typename TT2>
		void multiply(InfixExpression<TT1, TT2> &expression);

		NDArrayNode operator[](uint32_t);
		friend class NDArrayNode;
		void multiply1(InfixExpression<NDArray, NDArray> &expression);
		void add(InfixExpression<NDArray, NDArray> &expression);
		void subtract(InfixExpression<NDArray, NDArray> &expression);
		void divide(InfixExpression<NDArray, NDArray> &expression);
		NDArray & operator=(InfixExpression<NDArray, NDArray> expression);
		NDArray & operator=(InfixExpression<NDArrayTransposedView, NDArray> expression);
		InfixExpression<NDArray, NDArray> operator*(NDArray &right);
		InfixExpression<NDArray, NDArray> operator+(NDArray &right);
		InfixExpression<NDArray, NDArray> operator-(NDArray &right);
		InfixExpression<NDArray, NDArray> operator/(NDArray &right);
		uint32_t getIndex(vector<uint32_t> indexes) const;
		void operator +=(T1 value);
		void operator -=(T1 value);
		void operator *=(T1 value);
		void operator /=(T1 value);
		void print_for_testing();
		vector<uint32_t> get_dimensions();
		static void fill(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 value);
		static void copy(NDArray &target_ndArray,vector<uint32_t> target_from,NDArray &source_ndArray,vector<uint32_t> source_from,vector<uint32_t> source_to);
		NDArrayTransposedView as_transposed_view(bool is_read_only);
		NDArrayView as_view(vector<uint32_t>& from, vector<uint32_t>& to);
	friend class NDArrayTransposedView;
	friend class NDArrayView;
};

template<typename... TT>
void NDArray::set(TT ...arguments)
{
vector<uint32_t> indexes;
double d;

(indexes.push_back((uint32_t)arguments),...);
((d=(double)arguments),...);

indexes.pop_back();
if(indexes.size()!=this->dimensions.size())
{
ostringstream oss;  
oss<<"Size of array is ";
for(auto j:this->dimensions) oss<<"["<<j<<"]";
if(this->dimensions.size()>1)
{
oss<<", expected "<<this->dimensions.size()<<" indexes, found "<<indexes.size();
}
else
{
oss<<", expected "<<this->dimensions.size()<<" index, found "<<indexes.size();
}
throw NDArrayException(oss.str());
}
int idx=0;
int multiplier;
for(int i=0;i<this->dimensions.size();++i)
{

	if(indexes[i]>=this->dimensions[i])
	{
ostringstream oss;
oss<<"Array index out of bounds ";
for(int ii=0;ii<indexes.size();ii++) oss<<"["<<indexes[ii]<<"]";
oss<<", size of array is ";
for(auto j:this->dimensions) oss<<"["<<j<<"]";
throw NDArrayException(oss.str());
	}
	
multiplier=1;
for(int j=i+1;j<indexes.size();j++)
{
	multiplier=multiplier*this->dimensions[j];
}
idx=idx+indexes[i]*multiplier;
}
_set(idx,d);
}

template<typename... TT>
T1 NDArray::get(TT ...arguments)
{
vector<uint32_t> indexes;

(indexes.push_back((uint32_t)arguments),...);

if(indexes.size()!=this->dimensions.size())
{
ostringstream oss;  
oss<<"Size of array is ";
for(auto j:this->dimensions) oss<<"["<<j<<"]";
if(this->dimensions.size()>1)
{
oss<<", expected "<<this->dimensions.size()<<" indexes, found "<<indexes.size();
}
else
{
oss<<", expected "<<this->dimensions.size()<<" index, found "<<indexes.size();
}
throw NDArrayException(oss.str());
}
int idx=0;
int multiplier;
for(int i=0;i<this->dimensions.size();++i)
{
	if(indexes[i]>=this->dimensions[i])
	{
ostringstream oss;
oss<<"Array index out of bounds ";
for(int ii=0;ii<indexes.size();ii++) oss<<"["<<indexes[ii]<<"]";
oss<<", size of array is ";
for(auto j:this->dimensions) oss<<"["<<j<<"]";
throw NDArrayException(oss.str());
	}
multiplier=1;
for(int j=i+1;j<indexes.size();j++)
{
	multiplier=multiplier*this->dimensions[j];
}
idx=idx+indexes[i]*multiplier;
}
return _get(idx);
}


template<typename TT1, typename TT2>
void NDArray::multiply(InfixExpression<TT1, TT2>&expression)
{
	cout<<"template multiply function"<<endl;
	auto left = expression.left;
	auto left_array = left->get_raw_collection().data();
	auto right = expression.right;
	auto right_array = right->get_raw_collection().data();
	uint32_t i;
	uint32_t left_matrix_rows;
	uint32_t left_matrix_columns;
	uint32_t left_matrix_len;
	int number_of_left_matrices;
	uint32_t right_matrix_rows;
	uint32_t right_matrix_columns;
	uint32_t right_matrix_len;
	int number_of_right_matrices;
	auto left_dimensions = left->get_dimensions();
	auto right_dimensions = right->get_dimensions();

	if (left_dimensions.size() >= 2 && right_dimensions.size() >= 2)
	{
		i = left_dimensions.size();
		left_matrix_rows = left_dimensions[i - 2];
		left_matrix_columns = left_dimensions[i - 1];
		left_matrix_len = left_matrix_rows * left_matrix_columns;
		if (i == 2)
			number_of_left_matrices = 1;
		else
		{
			number_of_left_matrices = 1;
			for (int j = 0; j < i - 2; ++j)
			{
				number_of_left_matrices *= left_dimensions[j];
			}
		}

		i = right_dimensions.size();
		right_matrix_rows = right_dimensions[i - 2];
		right_matrix_columns = right_dimensions[i - 1];
		right_matrix_len = right_matrix_rows * right_matrix_columns;
		if (i == 2)
			number_of_right_matrices = 1;
		else
		{
			number_of_right_matrices = 1;
			for (int j = 0; j < i - 2; ++j)
			{
				number_of_right_matrices *= right_dimensions[j];
			}
		}
	}
	else if (left_dimensions.size() == 1 && right_dimensions.size() == 1)
	{
		if (this->dimensions.size() == 1 && this->dimensions[0] == 1)
		{
			left_matrix_rows = 1;
			left_matrix_columns = left_dimensions[0];
			left_matrix_len = left_dimensions[0];
			number_of_left_matrices = 1;
			right_matrix_rows = right_dimensions[0];
			right_matrix_columns = 1;
			right_matrix_len = right_dimensions[0];
			number_of_right_matrices = 1;
		}
		else if (this->dimensions.size() >= 2)
		{
			left_matrix_rows = left_dimensions[0];
			left_matrix_columns = 1;
			left_matrix_len = left_dimensions[0];
			number_of_left_matrices = 1;
			right_matrix_rows = 1;
			right_matrix_columns = right_dimensions[0];
			right_matrix_len = right_dimensions[0];
			number_of_right_matrices = 1;
		}
		else if (left_dimensions[0] == 1)
		{
			left_matrix_rows = 1;
			left_matrix_columns = 1;
			left_matrix_len = 1;
			number_of_left_matrices = 1;
			right_matrix_rows = 1;
			right_matrix_columns = right_dimensions[0];
			right_matrix_len = right_dimensions[0];
			number_of_right_matrices = 1;
		}
		else if (right_dimensions[0] == 1)
		{
			left_matrix_rows = left_dimensions[0];
			left_matrix_columns = 1;
			left_matrix_len = left_dimensions[0];
			number_of_left_matrices = 1;
			right_matrix_rows = 1;
			right_matrix_columns = 1;
			right_matrix_len = 1;
			number_of_right_matrices = 1;
		}
	}
	else if (left_dimensions.size() == 1 && right_dimensions.size() >= 2)
	{
		left_matrix_rows = 1;
		left_matrix_columns = left_dimensions[0];
		left_matrix_len = left_matrix_rows * left_matrix_columns;
		number_of_left_matrices = 1;
		i = right_dimensions.size();
		right_matrix_rows = right_dimensions[i - 2];
		right_matrix_columns = right_dimensions[i - 1];
		right_matrix_len = right_matrix_rows * right_matrix_columns;
		number_of_right_matrices = 1;
	}
	else if (left_dimensions.size() >= 2 && right_dimensions.size() == 1)
	{
		right_matrix_rows = right_dimensions[0];
		right_matrix_columns = 1;
		right_matrix_len = right_matrix_rows * right_matrix_columns;
		number_of_right_matrices = 1;
		i = left_dimensions.size();
		left_matrix_rows = left_dimensions[i - 2];
		left_matrix_columns = left_dimensions[i - 1];
		left_matrix_len = left_matrix_rows * left_matrix_columns;
		number_of_left_matrices = 1;
	}

	// pre fill product with 0
	auto product_array = this->collection.data();
	auto product_matrix_len = left_matrix_rows * right_matrix_columns;
	for (i = 0; i < this->collection.size(); ++i)
		this->collection[i] = 0.0;

	int r, c, c1;
	auto left_matrix = left_array;
	auto right_matrix = right_array;
	auto product_matrix = product_array;
	auto cores = thread::hardware_concurrency();
	if (number_of_left_matrices >= cores)
	{
#pragma omp parallel for private(r, c, c1)
		for (i = 0; i < number_of_left_matrices; ++i)
		{
			for (r = 0; r < left_matrix_rows; ++r)
			{
				for (c = 0; c < left_matrix_columns; ++c)
				{
					for (c1 = 0; c1 < right_matrix_columns; ++c1)
					{
						product_matrix[r * right_matrix_columns + c1] += (left->get(r,c )* right->get(c, c1));
					}
				}
			}
		}
		// implicit barrier
	}
	else
	{
		for (i = 0; i < number_of_left_matrices; ++i)
		{
#pragma omp parallel for private(c, c1)
			for (r = 0; r < left_matrix_rows; ++r)
			{
				for (c = 0; c < left_matrix_columns; ++c)
				{
					for (c1 = 0; c1 < right_matrix_columns; ++c1)
					{
						product_matrix[r * right_matrix_columns + c1] += (left->get(r,c)* right->get(c, c1));
					}
				}
			}
			// implicit barrier
		} // else ends
		// multiplying one matrix with other ends here
		left_matrix += left_matrix_len;
		right_matrix += right_matrix_len;
		product_matrix += product_matrix_len;

	} // multiplying all matrices ends here

}