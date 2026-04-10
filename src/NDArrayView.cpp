
#include <NDArrayView.h>
NDArrayView::NDArrayView(NDArray *arr, const vector<uint32_t> &from, const vector<uint32_t> &to)
{
    this->_from = from;
    this->_to = to;
    int n = arr->dimensions.size();
    int m = 1;
    this->ndArray = arr;

    int left_change, right_change;
    for (int i = 0; i < n; ++i)
    {
        if (from[i] != to[i])
        {
            left_change = i;
        }
    }

    for (int i = n - 1; i >= 0; --i)
    {
        if (from[i] != to[i])
        {
            right_change = i;
        }
        if (i == 0)
        {
            break;
        }
    }

    for (int i = left_change; i <= right_change; ++i)
    {
        dimensions.push_back(from[i] - to[i]);
    }

    for (int i = n - 1; i >= 0; i--)
    {

        _multipliers[i] = m;
        m *= arr->dimensions[i];
        if (i == 0)
            break;
    }
    this->precomputed_index = 0;
    for (int i = 0; i < n; i++)
    {
        precomputed_index += _multipliers[i] * from[i];

        if (from[i] == to[i])
        {
            _multipliers[i] = 0;
        }
    }
}