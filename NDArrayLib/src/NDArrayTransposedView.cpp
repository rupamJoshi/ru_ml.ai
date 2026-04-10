#include<NDArrayTransposedView.h>
NDArrayTransposedView::NDArrayTransposedView(NDArray *ndArray,bool is_read_only)
	:ndArray(ndArray),is_read_only(is_read_only){}

vector<uint32_t> NDArrayTransposedView::get_dimensions()
{
	vector<uint32_t> dimensions = this->ndArray->get_dimensions();
	if(dimensions.size()>1)
		swap(dimensions[dimensions.size() -1 ], dimensions[dimensions.size() -2]);
	return dimensions;

}