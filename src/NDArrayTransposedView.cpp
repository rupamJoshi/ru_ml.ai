#include <NDArrayTransposedView.h>
NDArrayTransposedView::NDArrayTransposedView(NDArray *ndArray, bool is_read_only)
	: ndArray(ndArray), is_read_only(is_read_only) {}

vector<uint32_t> NDArrayTransposedView::get_dimensions()
{
	vector<uint32_t> dimensions;
	if (this->ndArray == nullptr)
		return dimensions;
	dimensions = this->ndArray->dimensions;
	if (dimensions.size() == 1)
		return dimensions;
	size_t sz = dimensions.size();
	auto r = dimensions[sz - 2];
	auto c = dimensions[sz - 1];
	dimensions[sz - 2] = c;
	dimensions[sz - 1] = r;
	return dimensions;
}

InfixExpression<NDArrayTransposedView, NDArray> NDArrayTransposedView::operator*(NDArray &right)
{
	return InfixExpression<NDArrayTransposedView, NDArray>(this, '*', &right);
}

InfixExpression<NDArrayTransposedView, NDArray> NDArrayTransposedView::operator+(NDArray &right)
{
	return InfixExpression<NDArrayTransposedView, NDArray>(this, '+', &right);
}

InfixExpression<NDArrayTransposedView, NDArray> NDArrayTransposedView::operator-(NDArray &right)
{
	return InfixExpression<NDArrayTransposedView, NDArray>(this, '-', &right);
}

InfixExpression<NDArrayTransposedView, NDArray> NDArrayTransposedView::operator/(NDArray &right)
{
	return InfixExpression<NDArrayTransposedView, NDArray>(this, '/', &right);
}

vector<uint32_t>& NDArrayTransposedView::get_raw_dimensions()
{
	return this->ndArray->dimensions;
}
vector<T1>& NDArrayTransposedView::get_raw_collection()
{
	return this->ndArray->collection;

}

void NDArrayTransposedView::operator+=(T1 value)
{
	for (int i = 0; i < this->ndArray->collection.size(); ++i)
	{
		this->ndArray->collection[i] += value;
	}
}
void NDArrayTransposedView::operator+=(uint32_t value)
{
	for (int i = 0; i < this->ndArray->collection.size(); ++i)
	{
		this->ndArray->collection[i] += value;
	}
}

void NDArrayTransposedView::operator-=(T1 value)
{
	for (int i = 0; i < this->ndArray->collection.size(); ++i)
	{
		this->ndArray->collection[i] -= value;
	}
}
void NDArrayTransposedView::operator*=(T1 value)
{
	for (int i = 0; i < this->ndArray->collection.size(); ++i)
	{
		this->ndArray->collection[i] *= value;
	}
}
void NDArrayTransposedView::operator/=(T1 value)
{
	for (int i = 0; i < this->ndArray->collection.size(); ++i)
	{
		this->ndArray->collection[i] /= value;
	}
}
