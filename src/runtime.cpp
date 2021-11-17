#include <iostream>
#include <cmath>

#include "runtime.hpp"
#include "exceptions.hpp"

bool Runtime::var_exists(const std::string& identifier)
{
	return std::any_of(variables.begin(), variables.end(), 
						[&identifier](auto& var) {
							return var.name == identifier;
						});
}

const std::vector<Variable>::iterator Runtime::var_find(const std::string& identifier)
{
	auto it = std::find_if(variables.begin(), variables.end(), 
						[&identifier](auto& var) {
							return var.name == identifier;
						});

	if(it == variables.end())
		throw UndeclaredVariableExcept(identifier);

	return it;
}

ValueType Runtime::resolve_var_type(const std::string& identifier)
{
	for(auto& var : variables)
	{
		if(var.name == identifier)
			return var.type;
	}

	throw UndeclaredVariableExcept(identifier);
}

const std::string Runtime::resolve_var_str(const std::string& identifier)
{
	if(resolve_var_type(identifier) != ValueType::STRING)
		throw TypeErrorExcept(identifier, ValueType::STRING, ValueType::NUMBER);

	return var_find(identifier)->val_string;
}

float Runtime::resolve_var_num(const std::string& identifier)
{
	if(resolve_var_type(identifier) != ValueType::NUMBER)
		throw TypeErrorExcept(identifier, ValueType::NUMBER, ValueType::STRING);

	return var_find(identifier)->val_number;
}

void Runtime::assign_var_num(const std::string& identifier, float num)
{
	// Because redeclaration of variables is allowed
	if(var_exists(identifier))
	{
		auto it = std::find_if(variables.begin(), variables.end(), 
								[&identifier](auto& var) {
									return identifier == var.name;
								});

		it->type = ValueType::NUMBER;
		it->val_number = num;
		it->val_string = "";
	}
	else
		variables.emplace_back(identifier, ValueType::NUMBER, "", num);
}

void Runtime::assign_var_str(const std::string& identifier, const std::string& str)
{
	// Because redeclaration of variables is allowed
	if(var_exists(identifier))
	{
		auto it = std::find_if(variables.begin(), variables.end(), 
								[&identifier](auto& var) {
									return identifier == var.name;
								});

		it->type = ValueType::NUMBER;
		it->val_number = 0;
		it->val_string = str;
	}
	else
		variables.emplace_back(identifier, ValueType::STRING, str, 0);
}

void Runtime::execute_instruction(const Instruction& instruction)
{
	instruction.keyword_ptr->func(*this, instruction.tokens);
}

void Runtime::jump_instructions(int new_instr_counter)
{
	if(new_instr_counter > instructions.size() || new_instr_counter < 1)
		throw JumpOutOfBoundsExcept(instruction_counter + 1, new_instr_counter);
	else
		instruction_counter = new_instr_counter - 2;	// -1 to account for increment after each instr
}

int Runtime::run()
{
	// reset everything
	instruction_counter = 0;
	variables.clear();

	for(;;)
	{
		if(instruction_counter >= instructions.size())
		{
			std::cout << "ERROR: Unexpected end of instructions!" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		try // Execute our instruction
		{
			execute_instruction(instructions[instruction_counter]);
		}
		catch(TypeErrorExcept& e)
		{
			std::cout << "ERROR: In line " << instruction_counter + 1 << ": "
					  << "Type mismatch. Argument '" << e.token_str
					  << "' has type " << resolve_ValueType_str(e.got)
					  << " but " << resolve_ValueType_str(e.expected)
					  << " was expected." << std::endl;
			std::exit(EXIT_FAILURE);
		}
		catch(UndeclaredVariableExcept& e)
		{
			std::cout << "ERROR: In line " << instruction_counter + 1 << ": "
					  << "Undefined variable '" << e.identifier_str
					  << "'" << std::endl;
			std::exit(EXIT_FAILURE);
		}
		catch(JumpOutOfBoundsExcept& e)
		{
			std::cout << "ERROR: In line " << e.from << ": "
					  << "Jump target '" << e.to << "' is out of bounds."
					  << std::endl;
			std::exit(EXIT_FAILURE);
		}

		// Increment instruction counter
		++instruction_counter;
	}

	return 0;
}
