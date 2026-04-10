#pragma once
#include<thread>
#include<iostream>
#include<vector>
#include<exception>
#include<numeric>
#include<stdint.h>
#include<unordered_set>
#include<sstream>

using namespace std;

class NDArrayException:public std::exception
{
	private:
		std::string message;
	public:
		NDArrayException(const std::string &);
		const char * what() const noexcept override;
};

NDArrayException::NDArrayException(const std::string &_message): message(_message){};
const char * NDArrayException::what() const noexcept 
{
	return this->message.c_str();
}

#define T1 double

class NDArrayNode;

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
		NDArray *reference_of;
		bool is_read_only;

		void _set(uint32_t,T1);
		T1 _get(uint32_t);
		NDArray()
		{
			this->reference_of=this;
			this->is_read_only=true;
		}
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
		void multiply(InfixExpression &expression)
		{
auto left=expression.left;
auto left_array=left->reference_of->collection.data(); 
auto right=expression.right;
auto right_array=right->reference_of->collection.data();
uint32_t i;
uint32_t left_matrix_rows;
uint32_t left_matrix_columns;
uint32_t left_matrix_len;
int number_of_left_matrices;
uint32_t right_matrix_rows;
uint32_t right_matrix_columns;
uint32_t right_matrix_len;
int number_of_right_matrices;

if(left->dimensions.size()>=2 && right->dimensions.size()>=2)
{
i=left->dimensions.size();
left_matrix_rows=left->dimensions[i-2];
left_matrix_columns=left->dimensions[i-1];
left_matrix_len=left_matrix_rows*left_matrix_columns;
if(i==2) number_of_left_matrices=1;
else
{
	number_of_left_matrices=1;
	for(int j=0;j<i-2;++j)
	{
		number_of_left_matrices*=left->dimensions[j];
	}
}

i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_len=right_matrix_rows*right_matrix_columns;
if(i==2) number_of_right_matrices=1;
else
{
	number_of_right_matrices=1;
	for(int j=0;j<i-2;++j)
	{
		number_of_right_matrices*=right->dimensions[j];
	}
}
}
else if(left->dimensions.size()==1 && right->dimensions.size()==1)
{
	if(this->dimensions.size()==1 && this->dimensions[0]==1)
	{
		left_matrix_rows=1;
		left_matrix_columns=left->dimensions[0];
		left_matrix_len=left->dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=right->dimensions[0];
		right_matrix_columns=1;
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(this->dimensions.size()>=2)
	{
		left_matrix_rows=left->dimensions[0];
		left_matrix_columns=1;
		left_matrix_len=left->dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=right->dimensions[0];
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(left->dimensions[0]==1)
	{
		left_matrix_rows=1;
		left_matrix_columns=1;
		left_matrix_len=1;
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=right->dimensions[0];
		right_matrix_len=right->dimensions[0];
		number_of_right_matrices=1;
	}else if(right->dimensions[0]==1)
	{
		left_matrix_rows=left->dimensions[0];
		left_matrix_columns=1;
		left_matrix_len=left->dimensions[0];
		number_of_left_matrices=1;
		right_matrix_rows=1;
		right_matrix_columns=1;
		right_matrix_len=1;
		number_of_right_matrices=1;
	}
}
else if(left->dimensions.size()==1 && right->dimensions.size()>=2)
{
left_matrix_rows=1;
left_matrix_columns=left->dimensions[0];
left_matrix_len=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
i=right->dimensions.size();
right_matrix_rows=right->dimensions[i-2];
right_matrix_columns=right->dimensions[i-1];
right_matrix_len=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
}
else if(left->dimensions.size()>=2 && right->dimensions.size()==1)
{
right_matrix_rows=right->dimensions[0];
right_matrix_columns=1;
right_matrix_len=right_matrix_rows*right_matrix_columns;
number_of_right_matrices=1;
i=left->dimensions.size();
left_matrix_rows=left->dimensions[i-2];
left_matrix_columns=left->dimensions[i-1];
left_matrix_len=left_matrix_rows*left_matrix_columns;
number_of_left_matrices=1;
}

			// pre fill product with 0
			auto product_array=this->reference_of->collection.data();
			auto product_matrix_len=left_matrix_rows*right_matrix_columns;
			for(i=0;i<this->reference_of->collection.size();++i) this->reference_of->collection[i]=0.0;

			int r,c,c1;
			auto left_matrix=left_array;
			auto right_matrix=right_array;
			auto product_matrix=product_array;
			auto cores=thread::hardware_concurrency();
			if(number_of_left_matrices>=cores)
			{
			#pragma omp parallel for private(r,c,c1) 
			for(i=0;i<number_of_left_matrices;++i)
			{
			for(r=0;r<left_matrix_rows;++r)
			{
				for(c=0;c<left_matrix_columns;++c)
				{
					for(c1=0;c1<right_matrix_columns;++c1)
					{
product_matrix[r*right_matrix_columns+c1]+=(left_matrix[r*left_matrix_columns+c]*right_matrix[c*right_matrix_columns+c1]);
					}
				}
			
			}
			}
			// implicit barrier
			}
			else
			{
			for(i=0;i<number_of_left_matrices;++i)
			{
			#pragma omp parallel for private(c,c1)
			for(r=0;r<left_matrix_rows;++r)
			{
				for(c=0;c<left_matrix_columns;++c)
				{
					for(c1=0;c1<right_matrix_columns;++c1)
					{
product_matrix[r*right_matrix_columns+c1]+=(left_matrix[r*left_matrix_columns+c]*right_matrix[c*right_matrix_columns+c1]);
					}
				}
			
			}
			// implicit barrier
			} // else ends
			// multiplying one matrix with other ends here
			left_matrix+=left_matrix_len;
			right_matrix+=right_matrix_len;
			product_matrix+=product_matrix_len;

			} // multiplying all matrices ends here

		}

		void add(InfixExpression &expression)
		{
		}
		void subtract(InfixExpression &expression)
		{
		}
		void divide(InfixExpression &expression)
		{
		}



		NDArray & operator=(InfixExpression expression)
		{
			if(expression.oper=='*') this->multiply(expression);
			else if(expression.oper=='/') this->divide(expression);
			else if(expression.oper=='+') this->add(expression);
			else if(expression.oper=='-') this->subtract(expression);
			return *this;
		}


		InfixExpression operator*(NDArray &right)
		{
			return InfixExpression(this,'*',&right);
		}
		
		InfixExpression operator+(NDArray &right)
		{
			return InfixExpression(this,'+',&right);
		}
		
		InfixExpression operator-(NDArray &right)
		{
			return InfixExpression(this,'-',&right);
		}

		InfixExpression operator/(NDArray &right)
		{
			return InfixExpression(this,'/',&right);
		}




uint32_t getIndex(vector<uint32_t> indexes) const
{
uint32_t idx=0;
uint32_t multiplier;
for(int i=0;i<this->dimensions.size();++i)
{
multiplier=1;
for(int j=i+1;j<indexes.size();j++)
{
	multiplier=multiplier*this->dimensions[j];
}
idx=idx+indexes[i]*multiplier;
}
return idx;
}

void just_do_it(vector<uint32_t> from,vector<uint32_t> to)
{
int s=from.size();
vector<uint32_t> indices;
indices=from;
while(true)
{
for(int i=0;i<s;++i) cout<<indices[i]<<" ";
cout<<endl;
int i=s-1;
while(i>=0)
{
	indices[i]++;
	if(indices[i]<=to[i]) break;
	indices[i]=from[i];
	i--;
}
if(i<0) break;
}
}// just_do_it ends

void operator +=(T1 value)
{
	for(int i=0;i<this->reference_of->collection.size();++i) 
	{
		this->reference_of->collection[i]+=value;
	}
}

void operator -=(T1 value)
{
	for(int i=0;i<this->reference_of->collection.size();++i) 
	{
		this->reference_of->collection[i]-=value;
	}
}
void operator *=(T1 value)
{
	for(int i=0;i<this->reference_of->collection.size();++i) 
	{
		this->reference_of->collection[i]*=value;
	}
}
void operator /=(T1 value)
{
	for(int i=0;i<this->reference_of->collection.size();++i) 
	{
		this->reference_of->collection[i]/=value;
	}
}

void print_for_testing()
{
vector<uint32_t> from=this->dimensions;
vector<uint32_t> to=this->dimensions;
for(int i=0;i<from.size();++i)
{
	from[i]=0;
	to[i]--;
}
int s=from.size();
vector<uint32_t> indices;
indices=from;
while(true)
{
for(auto xx:indices) cout<<xx<<" ";
cout<<":";
cout<<this->reference_of->collection[this->getIndex(indices)];
cout<<endl;
int i=s-1;
while(i>=0)
{
	indices[i]++;
	if(indices[i]<=to[i]) break;
	indices[i]=from[i];
	i--;
}
if(i<0) break;
}
}// just_do_it ends


static void fill(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 value)
{
int s=from.size();
vector<uint32_t> indices;
indices=from;
while(true)
{
ndArray.reference_of->collection[ndArray.getIndex(indices)]=value;
int i=s-1;
while(i>=0)
{
	indices[i]++;
	if(indices[i]<=to[i]) break;
	indices[i]=from[i];
	i--;
}
if(i<0) break;
}
}	

static void copy(NDArray &target_ndArray,vector<uint32_t> target_from,NDArray &source_ndArray,vector<uint32_t> source_from,vector<uint32_t> source_to)
{
int s=source_from.size();
vector<uint32_t> source_indices;
vector<uint32_t> target_indices;
source_indices=source_from;
target_indices=target_from;
int target_offset=0;

target_offset=target_ndArray.dimensions.size()-source_ndArray.dimensions.size();

T1 value;
while(true)
{
value=source_ndArray.reference_of->collection[source_ndArray.getIndex(source_indices)];
target_ndArray.reference_of->collection[target_ndArray.getIndex(target_indices)]=value;

int i=s-1;
while(i>=0)
{
	source_indices[i]++;
	target_indices[i+target_offset]++;
	if(source_indices[i]<=source_to[i]) break;
	source_indices[i]=source_from[i];
	target_indices[i+target_offset]=target_from[i+target_offset];
	i--;
}
if(i<0) break;
}
}	

NDArray as_transposed_view(bool is_read_only)
{
	NDArray tmp;
	tmp.is_read_only=is_read_only;
	tmp.reference_of=this->reference_of;
	tmp.dimensions=this->reference_of->dimensions;
	auto sz=tmp.dimensions.size();
	if(tmp.dimensions.size()>1)
	{
		auto r=tmp.dimensions[sz-2];
		auto c=tmp.dimensions[sz-1];
		tmp.dimensions[sz-2]=c;
		tmp.dimensions[sz-1]=r;
	}
	return tmp;
}
};


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


NDArray::NDArray(const std::vector<uint32_t> &_dimensions)
{
this->is_read_only=false;
this->reference_of=this;
if(_dimensions.size()==0) throw NDArrayException("Size of array not specified");
int product=std::accumulate(_dimensions.begin(),_dimensions.end(),1,std::multiplies<uint32_t>());
if(product==0)
{
	ostringstream oss;
	oss<<"Size of array cannot be zero, specified ";
	for(auto j:_dimensions) oss<<"["<<j<<"]";
	throw NDArrayException(oss.str());
}
this->dimensions=_dimensions;
this->collection.resize(product);
}


NDArray::NDArray(const NDArray &other) // copy constructor
{
if(other.reference_of==&other) this->reference_of=this;
else this->reference_of=other.reference_of;
this->is_read_only=other.is_read_only;
this->dimensions=other.dimensions;
this->collection=other.collection;
}


NDArray::NDArray(NDArray &&other)	// move constructor
{
if(other.reference_of==&other) this->reference_of=this;
else this->reference_of=other.reference_of;
this->is_read_only=other.is_read_only;
other.reference_of=&other;
other.is_read_only=true;
this->dimensions=std::move(other.dimensions);
this->collection=std::move(other.collection);
for(NDArrayNode *n:other.nodes) 
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
other.nodes.clear();
}


NDArray & NDArray::operator=(const NDArray &other) // copy assignment
{
for(NDArrayNode *n:this->nodes)
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
this->nodes.clear();
if(other.reference_of==&other) this->reference_of=this;
else this->reference_of=other.reference_of;
this->is_read_only=other.is_read_only;

this->dimensions=other.dimensions;
this->collection=other.collection;
return *this;
}
NDArray & NDArray::operator=(NDArray &&other) // move assignment
{
for(NDArrayNode *n:this->nodes)
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
this->nodes.clear();
if(other.reference_of==&other) this->reference_of=this;
else this->reference_of=other.reference_of;
this->is_read_only=other.is_read_only;
other.reference_of=&other;
other.is_read_only=true;
this->dimensions=std::move(other.dimensions);
this->collection=std::move(other.collection);
for(NDArrayNode *n:other.nodes) 
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
other.nodes.clear();
return *this;
}
NDArray::~NDArray()
{
for(NDArrayNode *n:this->nodes)
{
	n->ndArray=NULL;
	n->indexes.resize(0);
}
}

void NDArray::_set(uint32_t index,T1 value)
{
	if(this->is_read_only) return;
	this->reference_of->collection[index]=value;
}
T1 NDArray::_get(uint32_t index)
{
	return this->reference_of->collection[index];
}

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

NDArrayNode NDArray::operator[](uint32_t index)
{
	NDArrayNode node(this,{index}); 
	return node;
}

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
	
this->ndArray->reference_of->collection[this->getIndex()]=other.ndArray->collection[other.getIndex()];
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
	this->ndArray->reference_of->collection[this->getIndex()]=other.ndArray->collection[other.getIndex()];
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

	return this->ndArray->reference_of->collection[this->getIndex()];
}

T1 NDArrayNode::operator=(T1 value)
{
	if(this->ndArray==NULL)
	{
		throw NDArrayException(string("Invalid NDArrayNode, array does not exist."));
	}
this->ndArray->reference_of->collection[this->getIndex()]=value;
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



