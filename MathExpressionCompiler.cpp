#include "MathExpressionCompiler.h"
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/bind.hpp>


MathExpressionCompiler::MathExpressionCompiler()
	: m_code(),
	func_val_(), value_(), string_(), var_(), function_(), arguments_(), expr_(), factor_(), term_()
{
	m_code.clear();

	string_ = qi::lexeme['"' >> *~qi::char_('"') > '"']
		[
			boost::bind(&MathExpressionCompiler::compileString, &*this, _1)
		];
	string_.name("String");

	value_ = qi::double_
		[
			boost::bind(&MathExpressionCompiler::compileDouble, &*this, _1)
		]
	| string_;

	value_.name("Value");

	arguments_ = -(expr_ > *(',' > expr_));
	arguments_.name("Arguments");

	function_ = (qi::lexeme[+(qi::alpha | qi::char_('_'))] >> '(' > arguments_ > ')')[boost::bind(&MathExpressionCompiler::compileFunction, &*this, _1)];
	function_.name("Function");

	var_ = qi::lexeme[+(qi::alpha | qi::char_('_'))][boost::bind(&MathExpressionCompiler::compileVar, &*this, _1)];
	var_.name("Variable");

	func_val_ = value_ | function_ | var_;
	
	expr_ = term_ >> 
		*(
			('+' > term_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_ADD)]
		  | ('-' > term_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_SUB)]);
	expr_.name("Expression");

	factor_ = func_val_
		| '(' > expr_ > ')';
	factor_.name("Factor");

	term_ = factor_
		>> *(
			('*' > factor_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_MUL)]
		||  ('/' > factor_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_DIV)]);
	term_.name("Term");

	qi::on_error<qi::fail>(expr_,
		phoenix::bind(&MathExpressionCompiler::onCompileError, this, qi::_4));
}


MathExpressionCompiler::~MathExpressionCompiler()
{
}

vector<unsigned char> MathExpressionCompiler::compile(string code)
{
	bool result = qi::phrase_parse(code.begin(), code.end(), expr_, qi::space);
	if (!result)
	{
		throw CompileException("UNKNOWN");
	}
	return m_code;
}

void MathExpressionCompiler::compileDouble(double value)
{
	compileOpcode(OPCODE_PUSH_DOUBLE);
	seriallize((unsigned char*)&value, sizeof(value));
}

void MathExpressionCompiler::compileString(vector<char>& str)
{
	compileOpcode(OPCODE_PUSH_STRING);
	size_t opcodeSize = str.size() + sizeof(StringOpcode::str_size);
	StringOpcode* opcode = (StringOpcode*) new char[opcodeSize];
	opcode->str_size = (uint16_t)str.size();
	memcpy(opcode->str, &str[0], str.size());
	seriallize((unsigned char *)opcode, opcodeSize);
	delete opcode;
}

void MathExpressionCompiler::compileOpcode(BaseOpcode opcode)
{
	seriallize((unsigned char*)&opcode, sizeof(opcode));
}

void MathExpressionCompiler::compileFunction(vector<char>& name)
{
	compileString(name);
	compileOpcode(OPCODE_CALL_FUNCTION);
}

void MathExpressionCompiler::compileVar(vector<char>& name)
{
	compileString(name);
	compileOpcode(OPCODE_VAR);
}

void MathExpressionCompiler::onCompileError(boost::spirit::info parsing_info)
{
	if (parsing_info.value.type() == typeid(boost::spirit::utf8_string))
	{
		throw CompileException(boost::get<string>(parsing_info.value));
	}
	throw CompileException(parsing_info.tag);
}

void MathExpressionCompiler::seriallize(unsigned char * value, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		m_code.push_back(value[i]);
	}
}
