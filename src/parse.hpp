#ifndef BISCUIT_PARSE_HPP_INCLUDED
#define BISCUIT_PARSE_HPP_INCLUDED

#include <exception>
#include <string>
#include <vector>

#include "keywords.hpp"
#include "common.hpp"

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
	std::string keyword_name, token_str;
	WrongTokenExcept(std::string _keyword_name, std::string _token_str, const TokenType& _expected, const TokenType& _got)
		: expected(_expected), got(_got), keyword_name(_keyword_name), token_str(_token_str) {}
};

struct TypeErrorExcept : public std::exception
{
	ValueType expected, got;
	std::string keyword_name, token_str;
	TypeErrorExcept(std::string _keyword_name, std::string _token_str, const ValueType& _expected, const ValueType& _got)
        : expected(_expected), got(_got), keyword_name(_keyword_name), token_str(_token_str) {}
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
	const Keyword * keyword_ptr;

public:
	// Try to parse a list of symbols as instructions
	Instruction(std::vector<Token>& _token_list);
    void print();
};

#endif
