#include <NDArray.h>
class NDArrayView
{
public:
    NDArrayView(NDArray *arr, const vector<uint32_t> from, const vector<uint32_t> to);
    vector<uint32_t> _from;
    vector<uint32_t> _to;
    uint32_t _calculated_index;
    vector<uint32_t> _multipliers;
    uint32_t precomputed_index;
    vector<uint32_t> dimensions;
    NDArray *ndArray;
		vector<uint32_t>& get_raw_dimensions();

    vector<uint32_t> get_dimensions()
    {
        return this->dimensions;
    };

    template <typename... TT>
    T1 get(TT... arguments)
    {
        vector<uint32_t> indexes;
        (indexes.push_back((uint32_t)arguments), ...);

        if (indexes.size() != this->dimensions.size())
        {
            throw NDArrayException("Invalid number of indices");
        }

        int idx = this->precomputed_index;

        for (size_t i = 0; i < indexes.size(); i++)
        {
            //cout<<"in this loop : "<<i<<" : "<<indexes[i]<<" "<<_multipliers[i]<<endl;
            if (indexes[i] >= this->dimensions[i])
                throw NDArrayException("Index out of bounds");
            idx += indexes[i] * _multipliers[i];
        }
        //cout<<"NDArrayView::get -> "<<idx<<endl;
        return ndArray->_get(idx);
    }

    friend NDArray;
};