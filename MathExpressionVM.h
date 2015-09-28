#pragma once

#include <vector>
#include <string>
#include <map>
#include <stack>
#include "MachineCode.h"
#include <boost/variant.hpp>

using std::vector;
using std::string;
using std::map;
using boost::variant;
using std::stack;

typedef stack<variant<double, string>> ValuesStack;

class MathExpressionVM
{
public:
	MathExpressionVM();
	virtual ~MathExpressionVM();
	double run(vector<unsigned char>& code);
	void setVar(string name, double value);

private:
	template <typename T>
	T* readFromCode(vector<unsigned char>& code, size_t& stackPointer);
	template <typename T>
	T pop(ValuesStack& valuesStack);
	map<string, double> m_vars;
};

class RuntimeException : public std::exception
{

};

class NameException : public RuntimeException
{
public:
	NameException(const string& name)
		: m_name(name)
	{

	}

	string m_name;
};

class BadOpcodeException : public RuntimeException
{
};

class BadPopException : public RuntimeException
{

};

class StackOverflowException : public RuntimeException
{

};

template<typename T>
inline T* MathExpressionVM::readFromCode(vector<unsigned char>& code, size_t& stackPointer)
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

template<typename T>
inline T MathExpressionVM::pop(ValuesStack& valuesStack)
{
	if (!valuesStack.empty())
	{
		auto return_value_variant = valuesStack.top();
		if (return_value_variant.type() == typeid(T))
		{
			valuesStack.pop();
			return boost::get<T>(return_value_variant);
		}
		else
		{
			throw BadPopException();
		}
	}
	else
	{
		throw StackOverflowException();
	}
}
