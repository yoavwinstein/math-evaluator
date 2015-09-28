#include "MathExpressionVM.h"
#include <boost/math/constants/constants.hpp>

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
	ValuesStack stack;

	try
	{
		opcode = *readFromCode<BaseOpcode>(code, codeStackPointer);
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
			double value = *readFromCode<double>(code, codeStackPointer);
			stack.push(value);
			break;
		}

		case OPCODE_PUSH_STRING:
		{
			StringOpcode * stringOpcode = readFromCode<StringOpcode>(code, codeStackPointer);
			stringOpcode->str_size;
			string value(stringOpcode->str, stringOpcode->str + stringOpcode->str_size);
			stack.push(value);
			
			codeStackPointer += stringOpcode->str_size;
			break;
		}

		case OPCODE_ADD:
		{
			double param2 = pop<double>(stack);
			double param1 = pop<double>(stack);

			stack.push(param1 + param2);
			break;
		}

		case OPCODE_SUB:
		{
			double param2 = pop<double>(stack);
			double param1 = pop<double>(stack);

			stack.push(param1 - param2);
			break;
		}

		case OPCODE_MUL:
		{
			double param2 = pop<double>(stack);
			double param1 = pop<double>(stack);

			stack.push(param1 * param2);
			break;
		}

		case OPCODE_DIV:
		{
			double param2 = pop<double>(stack);
			double param1 = pop<double>(stack);

			stack.push(param1 / param2);
			break;
		}

		case OPCODE_CALL_FUNCTION:
		{
			string functionName = pop<string>(stack);

			if (functionName == "sin")
			{
				double sinVal = pop<double>(stack);
				stack.push(sin(sinVal));
			}
			else if (functionName == "cos")
			{
				double cosVal = pop<double>(stack);
				stack.push(cos(cosVal));
			}
			else if (functionName == "tan")
			{
				double sinVal = pop<double>(stack);
				stack.push(tan(sinVal));
			}
			else if (functionName == "pow")
			{
				double power = pop<double>(stack);
				double base = pop<double>(stack);
				stack.push(pow(base, power));
			}
			else if (functionName == "pi")
			{
				stack.push(boost::math::constants::pi<double>());
			}
			else if (functionName == "e")
			{
				stack.push(boost::math::constants::e<double>());
			}
			else
			{
				throw NameException(functionName);
			}
			break;
		}

		case OPCODE_VAR:
		{
			string varName = pop<string>(stack);

			if (m_vars.count(varName) <= 0)
			{
				throw NameException(varName);
			}

			stack.push(m_vars[varName]);

			break;
		}

		case OPCODE_NOP:
			break;

		default:
			throw BadOpcodeException();
		}

		try
		{
			opcode = *readFromCode<BaseOpcode>(code, codeStackPointer);
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
