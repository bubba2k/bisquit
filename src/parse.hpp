#ifndef BISCUIT_PARSE_HPP_INCLUDED
#define BISCUIT_PARSE_HPP_INCLUDED

#include <string>
#include <vector>

#include "keywords.hpp"
#include "common.hpp"
#include "runtime.hpp"

struct Token
{
private:
	bool parse_as_keyword(const std::string& str);

	bool parse_as_literal(const std::string& str);
	
	// if a word is not a keyword or literal, it must be identifier
	bool parse_as_identifier(const std::string& str);

public:
	enum TokenType type;
	enum ValueType value_type;
	float val_number;
	std::string val_string;

	Token(const std::string& str);

	void print() const;
};

struct Instruction
{
	std::vector<Token> tokens;
	const Keyword * keyword_ptr;

	// Try to parse a list of symbols as instructions
	Instruction(std::vector<Token>& _token_list);
    void print();
};

std::vector<Instruction> parse_instructions(const std::string& code_arg);

#endif
