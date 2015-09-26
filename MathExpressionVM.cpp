#include "MathExpressionVM.h"
#include <boost/variant.hpp>
#include <stack>
#include <boost/math/constants/constants.hpp>

using boost::variant;
using std::stack;

MathExpressionVM::MathExpressionVM()
{
}


MathExpressionVM::~MathExpressionVM()
{
}

double MathExpressionVM::run(vector<unsigned char>& code)
{
	bool lastOpcode = false;
	BaseOpcode opcode = OPCODE_NOP;
	size_t codeStackPointer = 0;
	double result = 0;
	stack<variant<double, string>> stack;

	try
	{
		opcode = *pop<BaseOpcode>(code, codeStackPointer);
	}
	catch (StackOverflowException&)
	{
		lastOpcode = true;
	}
	while (!lastOpcode)
	{
		switch (opcode)
		{
		case OPCODE_PUSH_DOUBLE:
		{
			double value = *pop<double>(code, codeStackPointer);
			stack.push(value);
			break;
		}

		case OPCODE_PUSH_STRING:
		{
			StringOpcode * stringOpcode = pop<StringOpcode>(code, codeStackPointer);
			stringOpcode->str_size;
			string value(stringOpcode->str, stringOpcode->str + stringOpcode->str_size);
			stack.push(value);
			codeStackPointer += stringOpcode->str_size;
			break;
		}

		case OPCODE_ADD:
		{
			double param2 = boost::get<double>(stack.top());
			stack.pop();
			double param1 = boost::get<double>(stack.top());
			stack.pop();

			stack.push(param1 + param2);
			break;
		}

		case OPCODE_SUB:
		{
			double param2 = boost::get<double>(stack.top());
			stack.pop();
			double param1 = boost::get<double>(stack.top());
			stack.pop();

			stack.push(param1 - param2);
			break;
		}

		case OPCODE_MUL:
		{
			double param2 = boost::get<double>(stack.top());
			stack.pop();
			double param1 = boost::get<double>(stack.top());
			stack.pop();

			stack.push(param1 * param2);
			break;
		}

		case OPCODE_DIV:
		{
			double param2 = boost::get<double>(stack.top());
			stack.pop();
			double param1 = boost::get<double>(stack.top());
			stack.pop();

			stack.push(param1 / param2);
			break;
		}

		case OPCODE_CALL_FUNCTION:
		{
			string functionName = boost::get<string>(stack.top());
			stack.pop();

			if (functionName == "sin")
			{
				double sinVal = boost::get<double>(stack.top());
				stack.pop();
				stack.push(sin(sinVal));
			}
			else if (functionName == "cos")
			{
				double sinVal = boost::get<double>(stack.top());
				stack.pop();
				stack.push(cos(sinVal));
			}
			else if (functionName == "tan")
			{
				double sinVal = boost::get<double>(stack.top());
				stack.pop();
				stack.push(tan(sinVal));
			}
			else if (functionName == "pow")
			{
				double power = boost::get<double>(stack.top());
				stack.pop();
				double sinVal = boost::get<double>(stack.top());
				stack.pop();
				stack.push(pow(sinVal, power));
			}
			else if (functionName == "pi")
			{
				stack.push(boost::math::constants::pi<double>());
			}
			else if (functionName == "e")
			{
				stack.push(boost::math::constants::e<double>());
			}
			break;
		}

		case OPCODE_VAR:
		{
			string varName = boost::get<string>(stack.top());
			stack.pop();

			if (m_vars.count(varName) <= 0)
			{
				throw NameException();
			}

			stack.push(m_vars[varName]);

			break;
		}

		case OPCODE_NOP:
			break;
		}

		try
		{
			opcode = *pop<BaseOpcode>(code, codeStackPointer);
		}
		catch (StackOverflowException&)
		{
			lastOpcode = true;
		}
	}

	return boost::get<double>(stack.top());
}

void MathExpressionVM::setVar(string name, double value)
{
	m_vars[name] = value;
}
