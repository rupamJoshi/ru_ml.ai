#pragma once
#include <NDArray.h>
#include <NDArrayException.h>
using namespace std;
class NDArray; // important forward declaration
class NDArrayTransposedView
{
private:
	NDArray *ndArray;
	bool is_read_only;
	vector<uint32_t> &get_raw_dimensions();
	vector<T1> &get_raw_collection();

public:
	NDArrayTransposedView(NDArray *ndArray, bool is_read_only);
	vector<uint32_t> get_dimensions();

	template <typename... TT>
	void set(TT...);

	template <typename... TT>
	T1 get(TT...);
	void operator+=(T1 value);
	void operator-=(T1 value);
	void operator*=(T1 value);
	void operator/=(T1 value);
	void operator+=(uint32_t value);
	// void operator -=(T1 value);
	// void operator *=(T1 value);
	// void operator /=(T1 value);
	InfixExpression<NDArrayTransposedView, NDArray> operator*(NDArray &right);
	InfixExpression<NDArrayTransposedView, NDArray> operator+(NDArray &right);
	InfixExpression<NDArrayTransposedView, NDArray> operator-(NDArray &right);
	InfixExpression<NDArrayTransposedView, NDArray> operator/(NDArray &right);
	friend class NDArray;
};

template <typename... TT>
void NDArrayTransposedView::set(TT... arguments)
{
	if (this->ndArray == nullptr)
		return;
	if (this->is_read_only)
		return;
	vector<uint32_t> dimensions = this->get_dimensions();
	vector<uint32_t> indexes;
	double d;

	(indexes.push_back((uint32_t)arguments), ...);
	((d = (double)arguments), ...);

	indexes.pop_back();
	if (indexes.size() != dimensions.size())
	{
		ostringstream oss;
		oss << "Size of array is ";
		for (auto j : dimensions)
			oss << "[" << j << "]";
		if (dimensions.size() > 1)
		{
			oss << ", expected " << dimensions.size() << " indexes, found " << indexes.size();
		}
		else
		{
			oss << ", expected " << dimensions.size() << " index, found " << indexes.size();
		}
		throw NDArrayException(oss.str());
	}

	int i;
	for (i = 0; i < dimensions.size(); ++i)
	{

		if (indexes[i] >= dimensions[i])
		{
			ostringstream oss;
			oss << "Array index out of bounds ";
			for (int ii = 0; ii < indexes.size(); ii++)
				oss << "[" << indexes[ii] << "]";
			oss << ", size of array is ";
			for (auto j : dimensions)
				oss << "[" << j << "]";
			throw NDArrayException(oss.str());
		}
	}
	int idx = 0;
	int multiplier;

	if (indexes.size() > 1)
	{
		size_t sz = indexes.size();
		auto r = indexes[sz - 2];
		auto c = indexes[sz - 1];
		indexes[sz - 2] = c;
		indexes[sz - 1] = r;
	}
	auto ndArrayDimensions = this->ndArray->get_dimensions();
	for (i = 0; i < dimensions.size(); ++i)
	{
		multiplier = 1;
		for (int j = i + 1; j < indexes.size(); j++)
		{
			multiplier = multiplier * ndArrayDimensions[j];
		}
		idx = idx + indexes[i] * multiplier;
	}
	this->ndArray->_set(idx, d);
}

template <typename... TT>
T1 NDArrayTransposedView::get(TT... arguments)
{
	auto dimensions = this->get_dimensions();
	if (this->ndArray == nullptr)
		return 0.0;
	vector<uint32_t> indexes;

	(indexes.push_back((uint32_t)arguments), ...);

	if (indexes.size() != dimensions.size())
	{
		ostringstream oss;
		oss << "Size of array is ";
		for (auto j : dimensions)
			oss << "[" << j << "]";
		if (dimensions.size() > 1)
		{
			oss << ", expected " << dimensions.size() << " indexes, found " << indexes.size();
		}
		else
		{
			oss << ", expected " << dimensions.size() << " index, found " << indexes.size();
		}
		throw NDArrayException(oss.str());
	}
	int i;
	for (i = 0; i < dimensions.size(); ++i)
	{
		if (indexes[i] >= dimensions[i])
		{
			ostringstream oss;
			oss << "Array index out of bounds ";
			for (int ii = 0; ii < indexes.size(); ii++)
				oss << "[" << indexes[ii] << "]";
			oss << ", size of array is ";
			for (auto j : dimensions)
				oss << "[" << j << "]";
			throw NDArrayException(oss.str());
		}
	}
	int idx = 0;
	int multiplier;

	if (indexes.size() > 1)
	{
		size_t sz = indexes.size();
		auto r = indexes[sz - 2];
		auto c = indexes[sz - 1];
		indexes[sz - 2] = c;
		indexes[sz - 1] = r;
	}
	auto ndArrayDimensions = this->ndArray->get_dimensions();
	for (i = 0; i < dimensions.size(); ++i)
	{
		multiplier = 1;
		for (int j = i + 1; j < indexes.size(); j++)
		{
			multiplier = multiplier * ndArrayDimensions[j];
		}
		idx = idx + indexes[i] * multiplier;
	}
	auto value = this->ndArray->_get(idx);

	return value;
}
