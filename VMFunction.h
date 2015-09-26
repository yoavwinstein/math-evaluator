#include <string>
#include <stack>

using std::string;
using std::stack;

#pragma once
class VMFunction
{
public:
	VMFunction(const string& name, size_t parametersCount, void* functionPtr);
	size_t getParametersCount() const;
	const string& getName() const;
	double call(stack<double> parameters);
	virtual ~VMFunction();

private:
	string m_name;
	size_t m_parametersCount;
	void* m_functionPtr;
};

