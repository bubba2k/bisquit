#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "parse.hpp"

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
	std::ifstream infile = get_infile(argc, argv);
	std::string word;
    std::vector<Token> tokens;

	while(infile >> word)
	{
		try
		{
			Token token(word);
			tokens.push_back(token);
		}
		catch(MalformedIdentifierExcept& exc)
		{
			std::cout << "Malformed identifier: '" << exc.malformed_str << "'"
				      << "\nAborting..." << std::endl;
			exit(EXIT_FAILURE);
		}
		catch(UnknownKeywordExcept& exc)
		{
			std::cout << "Unknown Keyword: '" << exc.unknown_keyword << "'"
					  << "\nAborting..." << std::endl;
			exit(EXIT_FAILURE);
		}
		catch(WrongTokenExcept& exc)
		{
			std::cout << "Wrong symbol type\n Aborting..." << std::endl;
			exit(EXIT_FAILURE);
		}
	}

    Instruction instr(tokens);
    instr.print();

	return 0;
}
