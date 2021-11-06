#include <string>
#include <functional>

#include "common.hpp"
#include "keywords.hpp"

const std::vector<Keyword> keywords =
{
	{	
		.name = "print",
		.expected_num_args = -1,
		.expected_token_types = {TokenType::ID_OR_LIT},
		.expected_value_types = {ValueType::ANY}
	},
	{
		.name = "exit",
		.expected_num_args = 0
	}
};
