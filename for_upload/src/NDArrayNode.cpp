#include<NDArrayNode.h>
#include<NDArray.h>

NDArrayNode::NDArrayNode(NDArray *ndArray,const vector<uint32_t> &indexes)
{
this->ndArray=ndArray;
this->indexes=indexes;
this->ndArray->nodes.insert(this);
}

NDArrayNode::NDArrayNode(const NDArrayNode &other)
{
this->ndArray=other.ndArray;
this->indexes=other.indexes;
this->ndArray->nodes.insert(this);
}

NDArrayNode::NDArrayNode(NDArrayNode &&other)
{
this->ndArray=other.ndArray;
this->indexes=other.indexes;
this->ndArray->nodes.insert(this);
}

NDArrayNode::~NDArrayNode()
{
if(this->ndArray)this->ndArray->nodes.erase(this);
}


NDArrayNode & NDArrayNode::operator=(const NDArrayNode &other)
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Left operand NDarrayNode is invalid, array does not exist."));
	}
	if(other.ndArray==NULL)
	{
		throw NDArrayException(string("Right operand NDAraryNode is invalid, array does not exist."));
	}
	
this->ndArray->collection[this->getIndex()]=other.ndArray->collection[other.getIndex()];
	return *this;
}

NDArrayNode & NDArrayNode::operator=(NDArrayNode &&other)
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Left operand NDArrayNode is invalid, array does not exist."));
	}
	if(other.ndArray==NULL)
	{
		throw NDArrayException(string("Right operand NDArrayNode is invalid, array does not exist."));
	}
	this->ndArray->collection[this->getIndex()]=other.ndArray->collection[other.getIndex()];
	return *this;
}


NDArrayNode NDArrayNode::operator[](uint32_t index)
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}
	vector<uint32_t> vec;
	vec=this->indexes;
	vec.push_back(index);
	NDArrayNode node(this->ndArray,vec);
	return node;
}

NDArrayNode::operator T1()
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}

	return this->ndArray->collection[this->getIndex()];
}

T1 NDArrayNode::operator=(T1 value)
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}
this->ndArray->collection[this->getIndex()]=value;
return value;
}

uint32_t NDArrayNode::getIndex() const
{
if(this->ndArray->dimensions.size()!=this->indexes.size())
{ 	
ostringstream oss;
for(auto j:this->indexes) oss<<"["<<j<<"]";
oss<<" is invalid, size of array is ";
for(auto j:this->ndArray->dimensions) oss<<"["<<j<<"]";
throw NDArrayException(oss.str());
}


uint32_t idx=0;
uint32_t multiplier;
for(int i=0;i<this->ndArray->dimensions.size();++i)
{
if(this->indexes[i]>=this->ndArray->dimensions[i])
{
ostringstream oss;
oss<<"Array index out of bounds ";
for(auto j:this->indexes) oss<<"["<<j<<"]";
oss<<", size of array is ";
for(auto j:this->ndArray->dimensions) oss<<"["<<j<<"]";
throw NDArrayException(oss.str());
}
multiplier=1;
for(int j=i+1;j<this->indexes.size();j++)
{
	multiplier=multiplier*this->ndArray->dimensions[j];
}
idx=idx+this->indexes[i]*multiplier;
}
return idx;
}



