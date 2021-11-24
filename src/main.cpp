#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "common.hpp"
#include "exceptions.hpp"
#include "parse.hpp"
#include "runtime.hpp"

std::ifstream get_infile(int argc, char ** argv)
{
	if(argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <infile>" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::ifstream infile(argv[1]);

	if(!infile)
	{
		std::cerr << "Could not open '" << argv[1] << "'" << std::endl;
		exit(EXIT_FAILURE);
	}

	return infile;
}

int main(int argc, char * argv[])
{
    // Get the entire source code from the file
	std::ifstream infile = get_infile(argc, argv);
    std::stringstream buffer;
    buffer << infile.rdbuf();

    std::vector<Instruction> instructions = parse_instructions(buffer.str());

	Runtime rt(instructions);
	rt.run();

	return 0;
}
