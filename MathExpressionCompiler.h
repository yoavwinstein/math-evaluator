#pragma once

#include <vector>
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include "MachineCode.h"

using std::vector;
using std::stringstream;
using std::string;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

class MathExpressionCompiler
{
public:
	MathExpressionCompiler();
	virtual ~MathExpressionCompiler();

	vector<unsigned char> compile(string code);
private:
	void compileDouble(double value);
	void compileString(vector<char> & str);
	void compileOpcode(BaseOpcode opcode);
	void compileFunction(vector<char> & name);
	void compileVar(vector<char> & name);

	void onCompileError(boost::spirit::info parsing_info);
	void seriallize(unsigned char* value, size_t size);
	
	vector<unsigned char> m_code;
	stringstream m_error;
	qi::rule<std::string::iterator, qi::space_type>
		func_val_, value_, string_, var_, function_, arguments_, expr_, factor_, term_;
};

class CompileException : public std::exception
{
public:
	CompileException(const string& symbolError)
		: m_symbol(symbolError)
	{
		
	}
	string m_symbol;
};