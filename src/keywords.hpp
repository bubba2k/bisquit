#ifndef BISCUIT_KEYWORDS_HPP_INCLUDED
#define BISCUIT_KEYWORDS_HPP_INCLUDED

#include <string>
#include <vector>
#include <array>
#include "parse.hpp"

struct Keyword
{
	const std::string name;
	const int expected_num_args; // -1 means: 1 to infinity, in which case all args are of same type
	
	const std::vector<TokenType> expected_token_types;
	const std::vector<ValueType>  expected_value_types;
};

extern const std::vector<Keyword> keywords;

#endif
