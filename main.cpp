#include <iostream>
#include <boost/spirit/include/qi.hpp>
#include "MachineCode.h"
#include <boost/phoenix.hpp>
#include "MathExpressionCompiler.h"
#include <boost/variant.hpp>
#include "MathExpressionVM.h"
#include <map>

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

using std::cout;
using std::cin;

using std::vector;

void seriallize(vector<unsigned char> & code, unsigned char * value, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		code.push_back(value[i]);
	}
}

void func(vector<unsigned char> & code, double a)
{
	//seriallize(code, (unsigned char*)&a, sizeof(a));
}

void func2(std::vector<char>& a)
{
	std::cout << &a[0] << "\n";
}

int main()
{
	double var = 0;
	while (1)
	{
		std::string input;
		char buf[1024];
		cin.getline(buf, sizeof(buf));
		input = std::string(buf);
		vector<unsigned char>& code = vector<unsigned char>();

		MathExpressionCompiler comp;
		comp.compile(input);
	
		MathExpressionVM vm;
		vm.setVar("x", var);
		double res = vm.run(comp.m_code);
		std::cout << res << "\n";
		var += 1;
	}
	return 0;
}