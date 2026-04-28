#include <NDArrayView.h>
#include <vector>
#include <iostream>

using namespace std;

NDArrayView::NDArrayView(NDArray *arr, const vector<uint32_t> from, const vector<uint32_t> to)
{
    this->ndArray = arr;
    this->_from = from;
    this->_to = to;

    int n = arr->dimensions.size();

    int left_change = -1;
    int right_change = n - 1;

    for (int i = 0; i < n; ++i)
    {
        if (from[i] != to[i])
        {
            left_change = i;
            break;
        }
    }

    if (left_change == -1)
    {
        this->dimensions.push_back(1);
        this->_multipliers.push_back(1);

        this->precomputed_index = 0;
        uint32_t m = 1;

        for (int i = n - 1; i >= 0; --i)
        {
            this->precomputed_index += m * from[i];
            m *= arr->dimensions[i];
        }

        return;
    }

    if (n > 2)
    {
        // Rule: from first diff → till end
        right_change = n - 1;
    }
    else
    {
        // For 1D / 2D → only actual differing region
        right_change = left_change;

        for (int i = n - 1; i >= left_change; --i)
        {
            if (from[i] != to[i])
            {
                right_change = i;
                break;
            }
        }
    }

    this->dimensions.clear();

    for (int i = left_change; i <= right_change; ++i)
    {
        uint32_t val = (to[i] - from[i]) + 1;
        this->dimensions.push_back(val);
    }


    vector<uint32_t> original_strides(n);
    uint32_t stride_acc = 1;

    for (int i = n - 1; i >= 0; --i)
    {
        original_strides[i] = stride_acc;
        stride_acc *= arr->dimensions[i];
    }

    this->_multipliers.clear();

    for (int i = left_change; i <= right_change; ++i)
    {
        this->_multipliers.push_back(original_strides[i]);
    }

    this->precomputed_index = 0;
    uint32_t m = 1;

    for (int i = n - 1; i >= 0; --i)
    {
        this->precomputed_index += m * from[i];
        m *= arr->dimensions[i];
    }

    cout << "Range: [" << left_change << " to " << right_change << "]" << endl;
    cout << "Dimension size: " << this->dimensions.size() << endl;
}