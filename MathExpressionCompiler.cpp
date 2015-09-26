#include "MathExpressionCompiler.h"
#include <boost/phoenix/bind/bind_member_function.hpp>
#include <boost/bind.hpp>

MathExpressionCompiler::MathExpressionCompiler()
	: m_code(),
	func_val_(), value_(), string_(), var_(), func_call_(), arguments_(), expr_(), factor_(), term_()
{
	string_ = qi::lexeme['"' >> *~qi::char_('"') >> '"']
		[
			boost::bind(&MathExpressionCompiler::compileString, &*this, _1)
		];

	value_ = qi::double_
		[
			boost::bind(&MathExpressionCompiler::compileDouble, &*this, _1)
		]
	| string_;

	arguments_ = -(expr_ >> *(',' >> expr_));
	func_call_ = (qi::lexeme[+(qi::alpha | qi::char_('_'))] >> '(' >> arguments_ >> ')')[boost::bind(&MathExpressionCompiler::compileFunction, &*this, _1)];
		
	var_ = qi::lexeme[+(qi::alpha | qi::char_('_'))][boost::bind(&MathExpressionCompiler::compileVar, &*this, _1)];
	func_val_ = value_ | func_call_ | var_;
	
	expr_ = term_ >> 
		*(
			('+' >> term_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_ADD)]
		  | ('-' >> term_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_SUB)]);

	factor_ = func_val_
		| '(' >> expr_ >> ')';
	term_ = factor_
		>> *(
			('*' >> factor_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_MUL)]
		||  ('/' >> factor_)[boost::bind(&MathExpressionCompiler::compileOpcode, &*this, OPCODE_DIV)]);
}


MathExpressionCompiler::~MathExpressionCompiler()
{
}

void MathExpressionCompiler::compile(string code)
{
	bool result = qi::phrase_parse(code.begin(), code.end(), expr_, qi::space);
}

void MathExpressionCompiler::compileDouble(double value)
{
	compileOpcode(OPCODE_PUSH_DOUBLE);
	seriallize((unsigned char*)&value, sizeof(value));
	std::cout << "compiled a double" << value << "!\n";
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
	std::cout << "compiled a string!\n";
}

void MathExpressionCompiler::compileOpcode(BaseOpcode opcode)
{
	seriallize((unsigned char*)&opcode, sizeof(opcode));
	std::cout << "compiled an opcode" << opcode << "!\n";
}

void MathExpressionCompiler::compileFunction(vector<char>& name)
{
	compileString(name);
	compileOpcode(OPCODE_CALL_FUNCTION);
	std::cout << "compiled a function " << &name[0] << "!\n";
}

void MathExpressionCompiler::compileVar(vector<char>& name)
{
	compileString(name);
	compileOpcode(OPCODE_VAR);
	std::cout << "compiled a var " << &name[0] << "!\n";
}

void MathExpressionCompiler::seriallize(unsigned char * value, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		m_code.push_back(value[i]);
	}
}
