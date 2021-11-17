#ifndef BISCUIT_INTERPRETER_EXCEPTIONS_HPP
#define BISCUIT_INTERPRETER_EXCEPTIONS_HPP

#include <exception>
#include "common.hpp"

// Parsetime errors
struct MalformedTokenExcept : public std::exception
{
    std::string malformed_str;
    inline MalformedTokenExcept(const std::string& arg_str)
            : malformed_str(arg_str) {}
};

struct UnknownKeywordExcept : public std::exception
{
    std::string unknown_keyword;
    inline UnknownKeywordExcept(const std::string& arg_str)
            : unknown_keyword(arg_str) {}
};

struct WrongArgumentCountExcept : public std::exception
{
    int expected, got;
    std::string keyword_name;
    inline WrongArgumentCountExcept(const std::string& _name, int _expected, int _got)
            : expected(_expected), got(_got), keyword_name(_name) {}
};

struct WrongTokenExcept : public std::exception
{
    enum TokenType expected, got;
    std::string keyword_name, token_str;
    inline WrongTokenExcept(const std::string& _keyword_name, const std::string& _token_str, const enum TokenType& _expected, const enum TokenType& _got)
            : expected(_expected), got(_got), keyword_name(_keyword_name), token_str(_token_str) {}
};


// Runtime errors
struct TypeErrorExcept : public std::exception
{
    enum ValueType expected, got;
    std::string token_str;
    inline TypeErrorExcept(const std::string& _token_str, const enum ValueType& _expected, const enum ValueType& _got)
            : expected(_expected), got(_got), token_str(_token_str) {}
};

struct UndeclaredVariableExcept : public std::exception
{
	std::string identifier_str;
	inline UndeclaredVariableExcept(const std::string& str) : identifier_str(str) {}
};

struct JumpOutOfBoundsExcept : public std::exception
{
	int from, to;
	inline JumpOutOfBoundsExcept(int _from, int _to) : from(_from), to(_to) {}
};

#endif //BISCUIT_INTERPRETER_EXCEPTIONS_HPP
