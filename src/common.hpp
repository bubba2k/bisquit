#ifndef BISCUIT_INTERPRETER_COMMON_HPP
#define BISCUIT_INTERPRETER_COMMON_HPP

#include <string>

enum TokenType
{
    KEYWORD,
    IDENTIFIER,
    LITERAL,
    ID_OR_LIT
};

enum ValueType
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

const std::string resolve_TokenType_str(enum TokenType);
const std::string resolve_ValueType_str(enum ValueType);


#endif //BISCUIT_INTERPRETER_COMMON_HPP
