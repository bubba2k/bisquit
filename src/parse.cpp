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
		if(line[begin] == '"') // We have struck a string literal!!!
		{
			end = line.find("\"", begin + 1);

			if(end == std::string::npos)
			{
				std::cout << "ERROR: Unterminated string literal!" << std::endl;
				std::exit(EXIT_FAILURE);
			}

			end+=1;
			std::string word = line.substr(begin, end - begin);
        	tokens.emplace_back(word);

        	begin = line.find_first_not_of(blank_space, end);
		}
		else if(line[begin] == '#') // It's a comment.. discard rest of line!
		{
			break;
		}
		else
		{
        	end = line.find_first_of(blank_space, begin);
        	std::string word = line.substr(begin, end - begin);
        	tokens.emplace_back(word);

        	begin = line.find_first_not_of(blank_space, end);
		}
    }

    return std::move(tokens);
}

// Main parsing function
std::vector<Instruction> parse_instructions(const std::string& code_arg)
{
	unsigned int line_number = 1;

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
	
		try
		{
        	std::vector<Token> tokens = tokenize_line(line);
        	instructions.emplace_back(tokens);
		}
		catch(MalformedTokenExcept& e)
		{
			std::cout << "ERROR: In line " << line_number << ": "
					  << "Malformed identifier '" << e.malformed_str << "'"
					  << std::endl;
			std::exit(EXIT_FAILURE);
		}
		catch(UnknownKeywordExcept& e)
		{
			std::cout << "ERROR: In line " << line_number << ": "
				      << "Unknown keyword '" << e.unknown_keyword << "'"
					  << std::endl;
			std::exit(EXIT_FAILURE);
		}
		catch(WrongArgumentCountExcept& e)
		{
			std::cout << "ERROR: In line " << line_number << ": "
					  << "Wrong argument count. " << e.keyword_name
					  << " expects " << e.expected << " arguments but got "
					  << e.got << "." << std::endl;
			std::exit(EXIT_FAILURE);
		}
		catch(WrongTokenExcept &e)
		{
			std::cout << "ERROR: In line " << line_number << ": "
					  << "Token '" << e.token_str << "' is wrong kind of token. Keyword '" 
					  << e.keyword_name << "' expected " 
					  << resolve_TokenType_str(e.expected) << " but got " 
				      << resolve_TokenType_str(e.got) << "." << std::endl;

			std::exit(EXIT_FAILURE);
		}

        begin = code.find_first_not_of(line_end, end);

		++line_number;
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
	if(str[0] == '"' && str[str.size()-1] == '"')
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
	// First char must be alphabetic
	if(!std::isalpha(str[0])) return false;

	// Rest of identifier must be alphanumeric,
	// but can also contain underscores
	for(const char& c : str)
		if(!(std::isalnum(c)) && c != '_')
			return false;
	return true;
}


bool Token::parse_as_keyword(const std::string &str)
{
	auto str_cpy = str;
	for(char &c : str_cpy)
		c = (char) std::tolower(c);

	if(!is_keyword(str_cpy))
		return false;

	type = TokenType::KEYWORD;
	val_string = str_cpy;
		
	return true;
}


bool Token::parse_as_literal(const std::string &str)
{
	if(is_string_literal(str))
	{
		type = TokenType::LITERAL;
		value_type = ValueType::STRING;
		auto str_copy = str;

		// Erase the quotation marks
		str_copy.erase(0, 1);
		str_copy.erase(str.size()-2, 1);

		val_string = str_copy;
		return true;
	}
	else if(is_decimal_literal(str))
	{
		type = TokenType::LITERAL;
		value_type = ValueType::NUMBER;
		val_number = std::stof(str);
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
        value_type = ValueType::ANY;
		val_string = str;
        val_number  = 0.0f;
        return true;
	}
		return false;
}

Token::Token(const std::string &str)
{
    // First try to parse as str, then as literal, then as identifier
    if(!parse_as_keyword(str) && !parse_as_literal(str) && !parse_as_identifier(str))
		throw MalformedTokenExcept(str); // if all fails -> malformed token
}

void Token::print() const
{
	switch(type)
	{
		case TokenType::IDENTIFIER:
			std::cout << "Identifier: " << val_string;
			break;
		case TokenType::KEYWORD:
			std::cout << "Keyword: \"" << val_string << "\"";
			break;

		case TokenType::LITERAL:
			std::cout << "Literal of type ";
			if(value_type == ValueType::NUMBER)
				std::cerr << "Number: " << val_number;
			else if(value_type == ValueType::STRING)
				std::cout << "String: \"" << val_string << "\"";
			break;

		// Correctly parsed tokens should NEVER have this type:
		case TokenType::ID_OR_LIT:
			std::cout << "Something went wrong parsing this token!! "
					  << __FILE__ << " " << __LINE__;
	}
		
	std::cout << std::endl;
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
		switch(keyword_ptr->expected_token_types[i])
		{
			case TokenType::ID_OR_LIT:
				if(tokens[i+1].type != TokenType::IDENTIFIER && tokens[i+1].type != TokenType::LITERAL)
					throw WrongTokenExcept(keyword_ptr->name, tokens[i+1].val_string,
											keyword_ptr->expected_token_types[i],
											tokens[i+1].type);
				break;
			default:
				if(tokens[i+1].type != keyword_ptr->expected_token_types[i])
					throw WrongTokenExcept(keyword_ptr->name, tokens[i+1].val_string,
											keyword_ptr->expected_token_types[i],
											tokens[i+1].type);
				break;
		}
    }

    // if arbitrary number of args are expected, they're all one type
    if(keyword_ptr->expected_num_args == -1)
    {
        for (auto it = tokens.begin() + 1; it != tokens.end(); it++)
        {
			switch(keyword_ptr->expected_token_types[0])
			{
				case TokenType::ID_OR_LIT:
					if(it->type != TokenType::IDENTIFIER && it->type != TokenType::LITERAL)
						throw WrongTokenExcept(keyword_ptr->name, it->val_string,
											keyword_ptr->expected_token_types[0],
											it->type);
					break;
				default:
					if(it->type != keyword_ptr->expected_token_types[0])
						throw WrongTokenExcept(keyword_ptr->name, it->val_string,
											keyword_ptr->expected_token_types[0],
											it->type);
					break;
			}
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
