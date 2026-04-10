#pragma once
#include<stdexcept>
#include<string>
using namespace std;
class NDArrayException:public std::logic_error
{
	public:
		NDArrayException(const std::string &);
};

