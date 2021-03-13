#include "Compiler.hpp"

#include <regex>
#include <iostream>
#include <unordered_map>

AQACompiler::AQACompiler()
{}

AQACompiler::~AQACompiler()
{}

bool IsEmpty(std::string Str)
{
	if (Str.size() == 0) return true;
	if (Str == "") return true;

	bool bEmpty = true;

	for (auto Character : Str)
	{
		switch (Character)
		{
		case '\n':
		case '\t':
		case '\r':
		case '\b':
		case ' ':
			break;
		default:
			bEmpty = false;
			break;
		}

		if (!bEmpty)
			break;
	}

	return bEmpty;
}

std::vector<std::string> Tokenise(std::string Line)
{
	std::vector<std::string> Tokens;
	std::string Current, _temp;

	for (auto Character : Line)
	{
		switch (Character)
		{
		case '\r':
		case '\b':
			break;
		case '\n':
		case '\t':
		case ' ':
			if (!IsEmpty(Current)) 
			{
				Tokens.push_back(Current);
			}
			Current.clear();
			break;
		case '+':
		case '-':
		case '^':
		case '&':
		case '|':
		case '=':
		case '(':
		case ')':
		case '{':
		case '}':
		case ';':
			// Add current string.
			if (!IsEmpty(Current))
			{
				Tokens.push_back(Current);
			}
			Current.clear();
			
			// Add operator
			_temp.push_back(Character);
			Tokens.push_back(_temp);
			_temp.clear();
			break;
		default:
			Current.push_back(Character);
			break;
		}
	}
	if (!IsEmpty(Current))
	{
		Tokens.push_back(Current);
	}

	std::vector<std::string> Out;

	for (auto Str : Tokens)
	{
		if (!IsEmpty(Str))
		{
			Out.push_back(Str);
		}
	}

	return Tokens;
}

std::vector<std::string> ParseVariableLine(std::string Line)
{
	auto Tokens = Tokenise(Line);

	if (Tokens[0] != "let")
	{
		return {};
	}

	return {};
}

class RegisterMap
{
	class Register; // what

	std::vector<Register*> m_vecpRegisters;
	unsigned long long m_nRegistersUtilised = 0ULL;

public:

	class Register
	{
		std::string m_Name;
		unsigned long long m_uRegister;

	public:
		
		Register(std::string Name, unsigned long long Register)
			: m_Name(Name), m_uRegister(Register)
		{}

		~Register()
		{}

		std::string GetName() { return m_Name; }
		unsigned long long GetRegister() { return m_uRegister; }
	};

	RegisterMap()
		: m_vecpRegisters(std::vector<Register*>(12))
	{}

	~RegisterMap()
	{}

	void PushRegister(std::string Name)
	{
		m_vecpRegisters.push_back(new Register(Name, m_nRegistersUtilised));
		m_nRegistersUtilised++;
	}

	void PopRegister()
	{
		m_vecpRegisters.erase(m_vecpRegisters.end());
		m_nRegistersUtilised--;
	}

	Register* TopRegister()
	{
		return m_vecpRegisters[m_nRegistersUtilised - 1];
	}
};

std::string AQACompiler::CompileString(std::string Input)
{
	ParseVariableLine(Input);

	if (Input.size() == 0)
	{
		return {};
	}

	RegisterMap* pRegs = new RegisterMap();

	try {
		auto ValidLine				= std::regex_match(Input, std::regex("(.*);"));
		auto NewVariable			= std::regex_match(Input, std::regex("(let(\\t| |\\n)+)([A-Z]|[a-z]|_|[0-9])*(;*)"));
		auto NewVariableAssigned	= std::regex_match(Input, std::regex("((let(\\t| |\\n)+)([A-Z]|[a-z]|_|[0-9])*((\\t| |\\n)+=(\\t| |\\n)+)((([0-9]*)|(\\+|-|\\||&|\\^| |\\(|\\))*))*(([A-Z]|[a-z]|_|[0-9]||\\(|\\))+)(;*))"));

		if (!ValidLine)
		{
			std::cout << "Invalid line." << std::endl;
		}

		if (NewVariable)
		{
			std::cout << "New var." << std::endl;
		}

		else if (NewVariableAssigned)
		{
			std::cout << "New var assigned." << std::endl;
		}

		else
		{
			std::cout << "Unrecognised syntax." << std::endl;
		}
	}
	catch (std::regex_error E)
	{
		std::cout << E.what() << std::endl;
	}
	return "";
}