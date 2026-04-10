#include<NDArray.h>
using namespace std;

NDArray::NDArray()
{
}
vector<uint32_t> NDArray::get_dimensions()
{
	return this->dimensions;
}
void NDArray::multiply(InfixExpression &expression)
{
auto left=expression.left;
auto left_array=left->collection.data(); 
auto right=expression.right;
auto right_array=right->collection.data();
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
			auto product_array=this->collection.data();
			auto product_matrix_len=left_matrix_rows*right_matrix_columns;
			for(i=0;i<this->collection.size();++i) this->collection[i]=0.0;

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

		void NDArray::add(InfixExpression &expression)
		{
			uint32_t i=0;
			for(auto &t:this->collection)
			{
	t=expression.left->collection[i]+expression.right->collection[i];
			}
		}
		void NDArray::subtract(InfixExpression &expression)
		{
			uint32_t i=0;
			for(auto &t:this->collection)
			{
	t=expression.left->collection[i]/expression.right->collection[i];
			}
		}
			
		void NDArray::divide(InfixExpression &expression)
		{
			uint32_t i=0;
			for(auto &t:this->collection)
			{
	t=expression.left->collection[i]/expression.right->collection[i];
			}
		}

		NDArray & NDArray::operator=(InfixExpression expression)
		{
			if(expression.oper=='*') this->multiply(expression);
			else if(expression.oper=='/') this->divide(expression);
			else if(expression.oper=='+') this->add(expression);
			else if(expression.oper=='-') this->subtract(expression);
			return *this;
		}


		NDArray::InfixExpression NDArray::operator*(NDArray &right)
		{
			return InfixExpression(this,'*',&right);
		}
		
		NDArray::InfixExpression NDArray::operator+(NDArray &right)
		{
			return InfixExpression(this,'+',&right);
		}
		
		NDArray::InfixExpression NDArray::operator-(NDArray &right)
		{
			return InfixExpression(this,'-',&right);
		}

		NDArray::InfixExpression NDArray::operator/(NDArray &right)
		{
			return InfixExpression(this,'/',&right);
		}

uint32_t NDArray::getIndex(vector<uint32_t> indexes) const
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


void NDArray::operator +=(T1 value)
{
	for(int i=0;i<this->collection.size();++i) 
	{
		this->collection[i]+=value;
	}
}
void NDArray::operator -=(T1 value)
{
	for(int i=0;i<this->collection.size();++i) 
	{
		this->collection[i]-=value;
	}
}
void NDArray::operator *=(T1 value)
{
	for(int i=0;i<this->collection.size();++i) 
	{
		this->collection[i]*=value;
	}
}
void NDArray::operator /=(T1 value)
{
	for(int i=0;i<this->collection.size();++i) 
	{
		this->collection[i]/=value;
	}
}

void NDArray::print_for_testing()
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
cout<<this->collection[this->getIndex(indices)];
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
}


void NDArray::fill(NDArray &ndArray,vector<uint32_t> from,vector<uint32_t> to,T1 value)
{
int s=from.size();
vector<uint32_t> indices;
indices=from;
while(true)
{
ndArray.collection[ndArray.getIndex(indices)]=value;
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

void NDArray::copy(NDArray &target_ndArray,vector<uint32_t> target_from,NDArray &source_ndArray,vector<uint32_t> source_from,vector<uint32_t> source_to)
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
value=source_ndArray.collection[source_ndArray.getIndex(source_indices)];
target_ndArray.collection[target_ndArray.getIndex(target_indices)]=value;

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

NDArrayTransposedView NDArray::as_transposed_view(bool is_read_only)
{
	return NDArrayTransposedView(this,is_read_only);
}

NDArray::NDArray(const std::vector<uint32_t> &_dimensions)
{
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
this->dimensions=other.dimensions;
this->collection=other.collection;
}


NDArray::NDArray(NDArray &&other)	// move constructor
{
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
	this->collection[index]=value;
}
T1 NDArray::_get(uint32_t index)
{
	return this->collection[index];
}

NDArrayNode NDArray::operator[](uint32_t index)
{
	NDArrayNode node(this,{index}); 
	return node;
}



