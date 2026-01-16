#include <string>
#include <functional>
#include <iostream>
#include <cmath>

#include "common.hpp"
#include "keywords.hpp"

const std::vector<Keyword> keywords =
{
	{	
		.name = "print",
		.expected_num_args = -1,
		.expected_token_types = {TokenType::ID_OR_LIT},
		.expected_value_types = {ValueType::ANY},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			for(auto& arg : tokens)
			{
				if(arg.type == TokenType::LITERAL)
				{
					if(arg.value_type == ValueType::STRING)
						std::cout << arg.val_string;
					else
						std::cout << arg.val_number;
				}
				else if(arg.type == TokenType::IDENTIFIER)
				{
					if(rt.resolve_var_type(arg.val_string) == ValueType::STRING)
						std::cout << rt.resolve_var_str(arg.val_string);
					else if (rt.resolve_var_type(arg.val_string) == ValueType::NUMBER)
						std::cout << rt.resolve_var_num(arg.val_string);
				}
			}
			std::cout << std::endl;
		}
	},
	{
		.name = "exit",
		.expected_num_args = 0,
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			std::exit(EXIT_SUCCESS);
		}
	},
	{
		.name = "assign",
		.expected_num_args = 2,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::ANY, ValueType::ANY},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			if(tokens[0].type == TokenType::LITERAL)
			{
				if(tokens[0].value_type == ValueType::NUMBER)
					rt.assign_var_num(tokens[1].val_string, tokens[0].val_number);
				else
					rt.assign_var_str(tokens[1].val_string, tokens[0].val_string);
			}
			else if(tokens[0].type == TokenType::IDENTIFIER)
			{
				if(rt.resolve_var_type(tokens[0].val_string) == ValueType::NUMBER)
					rt.assign_var_num(tokens[1].val_string, rt.resolve_var_num(tokens[0].val_string));
				else
					rt.assign_var_str(tokens[1].val_string, rt.resolve_var_str(tokens[0].val_string));
			}
		}
	},
	{
		.name = "add",
		.expected_num_args = 3,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			float operands[2], res;

			// Retrieve operands
			for(unsigned int i = 0; i < 2; i++)
			{
				if(tokens[i].type == TokenType::LITERAL)
					operands[i] = tokens[i].val_number;
				else
					operands[i] = rt.resolve_var_num(tokens[i].val_string);
			}

			res = operands[0] + operands[1];

			rt.assign_var_num(tokens[2].val_string, res);
		}
	},
	{
		.name = "mul",
		.expected_num_args = 3,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			float operands[2], res;

			// Retrieve operands
			for(unsigned int i = 0; i < 2; i++)
			{
				if(tokens[i].type == TokenType::LITERAL)
					operands[i] = tokens[i].val_number;
				else
					operands[i] = rt.resolve_var_num(tokens[i].val_string);
			}

			res = operands[0] * operands[1];

			rt.assign_var_num(tokens[2].val_string, res);
		}
	},
	{
		.name = "div",
		.expected_num_args = 3,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			float operands[2], res;

			// Retrieve operands
			for(unsigned int i = 0; i < 2; i++)
			{
				if(tokens[i].type == TokenType::LITERAL)
					operands[i] = tokens[i].val_number;
				else
					operands[i] = rt.resolve_var_num(tokens[i].val_string);
			}

			res = operands[0] / operands[1];

			rt.assign_var_num(tokens[2].val_string, res);
		}
	},
	{
		.name = "sub",
		.expected_num_args = 3,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			float operands[2], res;

			// Retrieve operands
			for(unsigned int i = 0; i < 2; i++)
			{
				if(tokens[i].type == TokenType::LITERAL)
					operands[i] = tokens[i].val_number;
				else
					operands[i] = rt.resolve_var_num(tokens[i].val_string);
			}

			res = operands[0] - operands[1];

			rt.assign_var_num(tokens[2].val_string, res);
		}
	},
	{
		.name = "goto",
		.expected_num_args = 2,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			int dest;
			float condition;

			if(tokens[0].type == TokenType::LITERAL)
				dest = static_cast<int>(std::floor(tokens[0].val_number));
			else
				dest = static_cast<int>(std::floor(rt.resolve_var_num(tokens[0].val_string)));

			if(tokens[1].type == TokenType::LITERAL)
				condition = std::floor(tokens[1].val_number);
			else
				condition = std::floor(rt.resolve_var_num(tokens[1].val_string));

			if(condition > 0.0f)
				rt.jump_instructions(dest);
		}
	},
	{
		.name = "jump",
		.expected_num_args = 2,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			int offset, dest;
			float condition;

			if(tokens[0].type == TokenType::LITERAL)
				offset = static_cast<int>(std::floor(tokens[0].val_number));
			else
				offset = static_cast<int>(std::floor(rt.resolve_var_num(tokens[0].val_string)));

			if(tokens[1].type == TokenType::LITERAL)
				condition = std::floor(tokens[1].val_number);
			else
				condition = std::floor(rt.resolve_var_num(tokens[1].val_string));

			dest = rt.instruction_counter + offset;

			if(condition > 0.0f)
				rt.jump_instructions(dest);
		}
	},
	{
		.name = "strin",
		.expected_num_args = 2,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::STRING, ValueType::ANY},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			if(tokens[0].type == TokenType::LITERAL)
				std::cout << tokens[0].val_string << " ";
			else
				std::cout << rt.resolve_var_str(tokens[0].val_string) << " ";

			std::string str;
			std::cin >> str;

			rt.assign_var_str(tokens[1].val_string, str);
		}
	},
	{
		.name = "numin",
		.expected_num_args = 2,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::STRING, ValueType::ANY},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			if(tokens[0].type == TokenType::LITERAL)
				std::cout << tokens[0].val_string << " ";
			else
				std::cout << rt.resolve_var_str(tokens[0].val_string) << " ";

			float num;
			std::cin >> num;

			rt.assign_var_num(tokens[1].val_string, num);
		}
	},
	{
		.name = "rand",
		.expected_num_args = 2,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::NUMBER, ValueType::ANY},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			int limit;
			if(tokens[0].type == TokenType::LITERAL)
				limit = static_cast<int>(tokens[0].val_number);
			else
				limit = static_cast<int>(rt.resolve_var_num(tokens[0].val_string));

			int number = rand() % limit;

			rt.assign_var_num(tokens[1].val_string, static_cast<float>(number));
		}
	},
	{
		.name = "equal",
		.expected_num_args = 3,
		.expected_token_types = {TokenType::ID_OR_LIT, TokenType::ID_OR_LIT, TokenType::IDENTIFIER},
		.expected_value_types = {ValueType::NUMBER, ValueType::NUMBER, ValueType::NUMBER},
		.func = [](Runtime& rt, const std::vector<Token>& tokens)
		{
			float operands[2], res;

			// Retrieve operands
			for(unsigned int i = 0; i < 2; i++)
			{
				if(tokens[i].type == TokenType::LITERAL)
					operands[i] = tokens[i].val_number;
				else
					operands[i] = rt.resolve_var_num(tokens[i].val_string);
			}

			res = std::round(operands[0]) == std::round(operands[1]);

			rt.assign_var_num(tokens[2].val_string, res);
		}
	}
};
