#include <iostream>
#include <exception>
#include <array>

#include "keywords.hpp"
#include "parse.hpp"

bool is_keyword(std::string str)
{
	for(char &c : str)
		c = std::tolower(c);

	for(auto &keyword : keywords)
		if(str == keyword.name)
			return true;
	return 0;
}

bool is_string_literal(const std::string& str)
{
	if(str[0] == '#')
		return true;
	else
		return false;
}

bool is_decimal_literal(const std::string& str)
{
	for(const char &c : str)
		if(!std::isdigit(c) && c != '.')
			return false;

	return true;
}

bool is_identifier(const std::string& str)
{
	for(const char& c : str)
		if(!(std::isalpha(c) || c == '_'))
			return false;
	return true;
}


bool Token::parse_as_keyword(const std::string &str)
{
	if(!is_keyword(str))
		return false;

	type = TokenType::KEYWORD;
	val_string = str;
		
	return true;
}


bool Token::parse_as_literal(const std::string &str)
{
	if(is_string_literal(str))
	{
		type = TokenType::LITERAL;
		literal_type = ValueType::STRING;
		auto str_copy = str;
		str_copy.erase(0, 1);
		val_string = str_copy;
		return true;
	}
	else if(is_decimal_literal(str))
	{
		type = TokenType::LITERAL;
		literal_type = ValueType::DECIMAL;
		val_float = std::stof(str);
		val_string = str;
		return true;
	}
	else
	{
		return false;
	}
}

bool Token::parse_as_identifier(const std::string &str)
{
	if(is_identifier(str))
	{
		type = TokenType::IDENTIFIER;
		val_string = str;
	}
		return false;
}

Token::Token(const std::string &str)
{
	if     (parse_as_keyword(str));
	else if(parse_as_literal(str));
	else if(parse_as_identifier(str));
	else
	{
		throw MalformedIdentifierExcept(str);
	}
}

void Token::print() const
{
	switch(type)
	{
		case TokenType::IDENTIFIER:
			std::cout << "Identifier: \"" << val_string << "\"";
			break;
		case TokenType::KEYWORD:
			std::cout << "Keyword: \"" << val_string << "\"";
			break;

		case TokenType::LITERAL:
			std::cout << "Literal of type ";
			if(literal_type == ValueType::DECIMAL)
				std::cout << "decimal: " << val_float;
			else if(literal_type == ValueType::STRING)
				std::cout << "string: \"" << val_string << "\"";
			break;
		case TokenType::ID_OR_LIT:
			std::cerr << "Identifier or Literal?! Something went terribly wrong!!";
	}
		
	std::cout << std::endl;
}



Instruction::Instruction(std::vector<Token>& _token_list)
{
	tokens = std::move(_token_list);

	// resolve our keyword
	auto keyword_it = std::find(keywords.begin(), keywords.end(), tokens[0].val_string);

	// Throw if unknown
	if(keyword_it == keywords.end())
		throw UnknownKeywordExcept(tokens[0].val_string);
	else
		keyword_ptr = &*keyword_it;

	// Make sure the keyword gets the number of arguments it needs
	if(keyword_ptr->expected_num_args != -1 && (keyword_ptr->expected_num_args != tokens.size() - 1))
		throw WrongArgumentCountExcept(keyword_ptr->name, keyword_ptr->expected_num_args, tokens.size() - 1);

	
}
