#pragma once

#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include "MachineCode.h"

using std::vector;
using std::string;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

class MathExpressionCompiler
{
public:
	MathExpressionCompiler();
	virtual ~MathExpressionCompiler();

	void compile(string code);
	vector<unsigned char> m_code;
private:
	void compileDouble(double value);
	void compileString(vector<char> & str);
	void compileOpcode(BaseOpcode opcode);
	void compileFunction(vector<char> & name);
	void compileVar(vector<char> & name);

	void seriallize(unsigned char* value, size_t size);
	
	qi::rule<std::string::iterator, qi::space_type>
		func_val_, value_, string_, var_, func_call_, arguments_, expr_, factor_, term_;
};

