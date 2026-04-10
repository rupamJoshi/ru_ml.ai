#pragma once
#include<thread>
#include<iostream>
#include<vector>
#include<exception>
#include<numeric>
#include<stdint.h>
#include<unordered_set>
#include<sstream>
#include<NDArrayNode.h>
#include<NDArrayTransposedView.h>
#include<NDArrayException.h>
#define T1 double
using namespace std;
class NDArrayNode;
class NDArrayTransposedView;
class NDArray
{
	private:
		class InfixExpression
		{
			NDArray *left;
			NDArray *right;
			char oper;
			InfixExpression(NDArray *left,char oper,NDArray *right)
			{
				this->left=left;
				this->right=right;
				this->oper=oper;
			}
		friend class NDArray;			
		};

		std::vector<T1> collection;
		std::vector<uint32_t> dimensions;
		std::unordered_set<NDArrayNode *> nodes; 

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

		NDArrayNode operator[](uint32_t);
		friend class NDArrayNode;
		void multiply(InfixExpression &expression);
		void add(InfixExpression &expression);
		void subtract(InfixExpression &expression);
		void divide(InfixExpression &expression);
		NDArray & operator=(InfixExpression expression);
		InfixExpression operator*(NDArray &right);
		InfixExpression operator+(NDArray &right);
		InfixExpression operator-(NDArray &right);
		InfixExpression operator/(NDArray &right);
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


