#pragma once

#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string>

class AQACompiler
{
public:

	AQACompiler();
	~AQACompiler();

	std::string CompileString(std::string Input);
};

#endif