#ifndef BISCUIT_KEYWORDS_HPP_INCLUDED
#define BISCUIT_KEYWORDS_HPP_INCLUDED

#include <string>
#include <vector>
#include <array>
#include <functional>

#include "parse.hpp"
#include "common.hpp"
#include "runtime.hpp"
#include "parse.hpp"

struct Keyword
{
	const std::string name;
	const int expected_num_args; // -1 means: 1 to infinity, in which case all args are of same type
	
	const std::vector<enum TokenType> expected_token_types;
	const std::vector<enum ValueType>  expected_value_types;

	std::function<void (Runtime&,  const std::vector<Token>&)> func;
};

extern const std::vector<Keyword> keywords;

#endif
