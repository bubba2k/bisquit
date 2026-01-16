#ifndef BISCUIT_INTERPRETER_COMMON_HPP
#define BISCUIT_INTERPRETER_COMMON_HPP

#include <string>

enum class TokenType
{
    KEYWORD,
    IDENTIFIER,
    LITERAL,
    ID_OR_LIT
};

enum class ValueType
{
    STRING,
    NUMBER,
    ANY
};

// some prototypes
struct Token;
struct Instruction;
struct Keyword;
struct Runtime;

std::string resolve_TokenType_str(TokenType);
std::string resolve_ValueType_str(ValueType);


#endif //BISCUIT_INTERPRETER_COMMON_HPP
