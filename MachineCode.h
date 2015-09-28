#include <cstdint>
#pragma once

// CR: Use enum class
enum BaseOpcode
{
	OPCODE_NOP = 0,
	OPCODE_PUSH_STRING,
	OPCODE_PUSH_DOUBLE,
	OPCODE_CALL_FUNCTION,
	OPCODE_ADD,
	OPCODE_SUB,
	OPCODE_MUL,
	OPCODE_DIV,
	OPCODE_VAR
};

struct StringOpcode
{
	uint16_t str_size;
	char str[0];
};