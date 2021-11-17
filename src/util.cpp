#include "common.hpp"
#include <string>

const std::string resolve_TokenType_str(enum TokenType ttype)
{
	switch(ttype)
	{
		case TokenType::KEYWORD:
			return "Keyword";
			break;
		case TokenType::IDENTIFIER:
			return "Identifier";
			break;
		case TokenType::LITERAL:
			return "Literal";
			break;
		case TokenType::ID_OR_LIT:
			return "Identifer or Literal";
			break;
	}

	return "UNKNOW TOKENTYPE";
}

const std::string resolve_ValueType_str(enum ValueType vtype)
{
	switch(vtype)
	{
		case ValueType::STRING:
			return "String";
			break;
		case ValueType::NUMBER:
			return "Number";
			break;
		case ValueType::ANY:
			return "Any";
			break;
	}

	return "UNKNOWN VALUETYPE";
}
