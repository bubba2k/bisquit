#include <iostream>
#include <exception>
#include <algorithm>
#include <vector>

#include "exceptions.hpp"
#include "keywords.hpp"
#include "parse.hpp"
#include "common.hpp"

static const std::string blank_space = "\t ";
static const std::string line_end = "\n";

// turn a line (one instruction) into tokens
std::vector<Token> tokenize_line(const std::string& line)
{
    std::vector<Token> tokens;

    // find all individual tokens
    std::size_t end, begin = line.find_first_not_of(blank_space, 0);
    while(begin != std::string::npos)
    {
        end = line.find_first_of(blank_space, begin);
        std::string word = line.substr(begin, end - begin);
        tokens.emplace_back(word);

        begin = line.find_first_not_of(blank_space, end);
    }

    return std::move(tokens);
}

std::vector<Instruction> parse_instructions(const std::string& code_arg)
{
    // Make a copy of the code and append a line_end symbol to make sure the
    // parser doesn't break on EOF
    std::string code = code_arg;
    code.append(line_end);

    std::vector<Instruction> instructions;

    std::size_t end, begin = code.find_first_not_of(line_end, 0);
    while(begin != std::string::npos)
    {
        end = code.find_first_of(line_end, begin);
        std::string line = code.substr(begin, end - begin);
        std::vector<Token> tokens = tokenize_line(line);
        instructions.emplace_back(tokens);

        begin = code.find_first_not_of(line_end, end);
    }

    return std::move(instructions);
}

bool is_keyword(std::string str)
{
	for(char &c : str)
		c = (char) std::tolower(c);

    return std::any_of(keywords.begin(), keywords.end(),[&str](auto& keyword){
        return keyword.name == str;
    });

	/* for(auto &keyword : keywords)
		if(str == keyword.name)
			return true;
	return 0;
	 */
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
		literal_type = ValueType::NUMBER;
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
        literal_type = ValueType::ANY;
		val_string = str;
        val_float  = 0.0f;
        return true;
	}
		return false;
}

Token::Token(const std::string &str)
{
    // First try to parse as str, then as literal, then as identifier
    if(!(parse_as_keyword(str) && parse_as_literal(str) && parse_as_identifier(str)))
		throw MalformedTokenExcept(str); // if all fails -> malformed token
}

void Token::print() const
{
	switch(type)
	{
		case TokenType::IDENTIFIER:
			std::cerr << "Identifier: \"" << val_string << "\"";
			break;
		case TokenType::KEYWORD:
			std::cerr << "Keyword: \"" << val_string << "\"";
			break;

		case TokenType::LITERAL:
			std::cerr << "Literal of type ";
			if(literal_type == ValueType::NUMBER)
				std::cerr << "decimal: " << val_float;
			else if(literal_type == ValueType::STRING)
				std::cerr << "string: \"" << val_string << "\"";
			break;
		case TokenType::ID_OR_LIT:
			std::cerr << "Identifier or Literal?! Something went terribly wrong!!";
	}
		
	std::cerr << std::endl;
}

Instruction::Instruction(std::vector<Token>& _token_list)
{
    // move over the tokens
	tokens = std::move(_token_list);

    // resolve our keyword
    auto keyword_it = std::find_if(keywords.begin(), keywords.end(), [&, this](auto& v){
        if(v.name == tokens[0].val_string)
            return true;
        else
            return false;
    });

	// Throw if unknown
	if(keyword_it == keywords.end())
		throw UnknownKeywordExcept(tokens[0].val_string);
	else
		keyword_ptr = &(*keyword_it);

	// Make sure the keyword gets the number of arguments it needs
	if(keyword_ptr->expected_num_args != -1 && (keyword_ptr->expected_num_args != tokens.size() - 1))
		throw WrongArgumentCountExcept(keyword_ptr->name, keyword_ptr->expected_num_args, tokens.size() - 1);

	// Check the arguments supplied are the right kind of tokens
    for(int i = 0; i < keyword_ptr->expected_num_args; i++)
    {
        if(keyword_ptr->expected_token_types[i] != tokens[i+1].type)
            throw WrongTokenExcept(keyword_ptr->name, tokens[i+1].val_string, keyword_ptr->expected_token_types[i],
                                   tokens[i+1].type);
    }

    // if arbitrary number of args are expected, they're all one type
    if(keyword_ptr->expected_num_args == -1)
    {
        for (auto it = tokens.begin() + 1; it != tokens.end(); it++)
        {
            if (keyword_ptr->expected_token_types[0] != it->type)
                throw WrongTokenExcept(keyword_ptr->name, it->val_string, keyword_ptr->expected_token_types[0],
                                       it->type);
        }
    }

    // If all checks succeeded, we consider this Instruction valid. Note that type checking occurs later at
    // runtime, when the instruction is executed.
    // We now get rid of the keyword token to save some space.
    tokens.erase(tokens.begin());
}

void Instruction::print()
{
    std::cerr << "INSTRUCTION: " << keyword_ptr->name << "\n";
    for(auto& token : tokens)
        token.print();

    std::cerr << std::endl;
}
