#ifndef BISCUIT_RUNTIME_HPP
#define BISCUIT_RUNTIME_HPP

#include <string>
#include "common.hpp"
#include "parse.hpp"

struct Variable
{
	std::string name;
	ValueType type;

	std::string val_string;
	float val_number;

	Variable(const std::string& _name, ValueType _type, const std::string& _string, float _number)
		: name(_name), type(_type), val_string(_string), val_number(_number) {}
};

struct Runtime
{
	int instruction_counter;
	std::vector<Instruction> instructions;
	std::vector<Variable> variables;

	inline Runtime(const std::vector<Instruction>& _instructions)
		: instruction_counter(1), instructions{_instructions} {}

	const std::vector<Variable>::iterator var_find(const std::string& identifier);
	bool 	  var_exists(const std::string& identifier);
	ValueType resolve_var_type(const std::string& identifier);
	const std::string resolve_var_str(const std::string& identifier);
	float resolve_var_num(const std::string& identifier);

	void assign_var_num(const std::string& identifier, float num);
	void assign_var_str(const std::string& identifier, const std::string& str);

	void jump_instructions(int new_instr_counter);

	void execute_instruction(const Instruction& instruction);

	int run();
};

#endif
