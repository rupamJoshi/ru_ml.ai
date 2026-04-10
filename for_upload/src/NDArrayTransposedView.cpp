#include<NDArrayTransposedView.h>
NDArrayTransposedView::NDArrayTransposedView(NDArray *ndArray,bool is_read_only)
	:ndArray(ndArray),is_read_only(is_read_only){}

