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
		std::cout << ">>> ";
		cin.getline(buf, sizeof(buf));
		input = std::string(buf);
		try
		{
			MathExpressionCompiler comp;
			vector<unsigned char> code =
			comp.compile(input);

			MathExpressionVM vm;
			vm.setVar("x", var);
			double res = vm.run(code);
			std::cout << res << "\n";
			var += 1;
		}
		catch (CompileException & ex)
		{
			std::cout << "Unable to compile. Expected: " << ex.m_symbol << "\n";
		}
		catch (NameException & ex)
		{
			std::cout << "Unknown name: " << ex.m_name << "\n";
		}
		catch (StackOverflowException & ex)
		{
			std::cout << "Stack overflow: " << "\n";
		}
	}
	return 0;
}