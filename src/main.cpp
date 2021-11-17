#if defined(WIN32) || defined(WIN32) || defined(__WIN32)
#include <Windows.h>
#endif

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

#include "common.hpp"
#include "exceptions.hpp"
#include "parse.hpp"
#include "runtime.hpp"

bool request_console();

// Must request console on Windows
#if defined(WIN32) || defined(WIN32) || defined(__WIN32)
bool request_console()
{
    return AllocConsole();
}
#else
bool request_console()
{
    return true;
}
#endif

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

    // On Windows, we have to explicitly request a console
    /*if(!request_console())
    {
        std::cerr << "Failed to get console!" << std::endl;
        exit(EXIT_FAILURE);
    }*/

    std::vector<Instruction> instructions = parse_instructions(buffer.str());

	Runtime rt(instructions);
	rt.run();

	return 0;
}
