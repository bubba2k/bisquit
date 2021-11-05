#ifndef BISCUIT_PARSE_HPP_INCLUDED
#define BISCUIT_PARSE_HPP_INCLUDED

#include <exception>
#include <string>
#include <vector>
#include "keywords.hpp"

enum TokenType
{
	KEYWORD,
	IDENTIFIER,
	LITERAL,
	ID_OR_LIT
};

enum ValueType
{
	STRING,
	DECIMAL,
	ANY
};

struct MalformedIdentifierExcept : public std::exception
{
	std::string malformed_str;
	MalformedIdentifierExcept(const std::string& arg_str)
		: malformed_str(arg_str) {}
};

struct UnknownKeywordExcept : public std::exception
{
	std::string unknown_keyword;
	UnknownKeywordExcept(const std::string& arg_str)
		: unknown_keyword(arg_str) {}
};

struct WrongArgumentCountExcept : public std::exception
{
	int expected, got;
	std::string keyword_name;
	WrongArgumentCountExcept(std::string _name, int _expected, int _got)
		: expected(_expected), got(_got), keyword_name(_name) {}
};

struct WrongTokenExcept : public std::exception
{
	TokenType expected, got;
	std::string keyword_name;
	WrongTokenExcept(std::string _name, const TokenType& _expected, const TokenType& _got)
		: expected(_expected), got(_got), keyword_name(_name) {}
};

struct TypeErrorExcept : public std::exception
{
	ValueType expected, got;
	std::string keyword_name;
	TypeErrorExcept(std::string _name, const ValueType& _expected, const ValueType& _got)
		: expected(_expected), got(_got), keyword_name(_name) {}
};


struct Token
{
private:
	bool parse_as_keyword(const std::string &str);

	bool parse_as_literal(const std::string &str);
	
	// if a word is not a keyword or literal, it must be identifier
	bool parse_as_identifier(const std::string &str);

public:
	enum TokenType type;
	enum ValueType literal_type;
	float val_float;
	std::string val_string;

	Token(const std::string &str);

	void print() const;
};

class Instruction
{
	std::vector<Token> tokens;
	Keyword *keyword_ptr;

public:
	// Try to parse a list of symbols as instructions
	Instruction(std::vector<Token>& _token_list);
};

#endif
