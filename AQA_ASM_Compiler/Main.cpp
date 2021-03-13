#include <iostream>

#include "Compiler.hpp"

int main(int argc, char** argv)
{
	while (true)
	{
		std::string In;
		std::getline(std::cin, In);

		AQACompiler().CompileString(In);
	}
}