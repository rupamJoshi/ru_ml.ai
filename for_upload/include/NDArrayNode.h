#pragma once
#include<NDArray.h>
#include<vector>
#define T1 double
using namespace std;
class NDArray; // important forward declaration
class NDArrayNode
{
	private:
		NDArray *ndArray;
		vector<uint32_t> indexes;
		NDArrayNode(NDArray *,const vector<uint32_t> &);

		NDArrayNode(const NDArrayNode &);
		NDArrayNode(NDArrayNode &&);

	public:
		NDArrayNode operator[](uint32_t);
		operator T1();
		T1 operator=(T1);
		NDArrayNode & operator=(const NDArrayNode &);
		NDArrayNode & operator=(NDArrayNode &&);
		~NDArrayNode();

		uint32_t getIndex() const;
	friend class NDArray;
};


