#pragma once

#include <vector>
#include <string>
#include <map>
#include "MachineCode.h"

using std::vector;
using std::string;
using std::map;

class MathExpressionVM
{
public:
	MathExpressionVM();
	virtual ~MathExpressionVM();
	double run(vector<unsigned char>& code);
	void setVar(string name, double value);

private:
	template <typename T>
	T* pop(vector<unsigned char>& code, size_t& stackPointer);

	map<string, double> m_vars;
};

class StackOverflowException : public std::exception
{

};

class NameException : public std::exception
{

};

template<typename T>
inline T* MathExpressionVM::pop(vector<unsigned char>& code, size_t& stackPointer)
{
	if (stackPointer + sizeof(T) > code.size())
	{
		throw StackOverflowException();
	}
	else
	{
		T* result = (T*)&code[stackPointer];
		stackPointer += sizeof(T);
		return result;
	}
}
