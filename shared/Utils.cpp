#include "Utils.hpp"

std::vector<std::string> Utils::Tokenise(std::string str)
{
	std::vector<std::string> Out;
	std::string Current, ColonStr;
	ColonStr.push_back(':');
	bool bExit = false;

	auto AddIfNotEmpty = [&Out](std::string& New)
	{
		if (New.size() != 0)
		{
			Out.push_back(New);
			New.clear();
		}
	};

	for (auto Character : str)
	{
		if (bExit) break;

		switch (Character)
		{
		case '\r':
		case '\t':
		case '\b':
		case ' ':
		case ',':
			AddIfNotEmpty(Current);
			break;
		case ':':
			AddIfNotEmpty(Current);
			Out.push_back(ColonStr);
			break;
		case '\n':
		case ';':
			bExit = true;
			break;
		default:
			Current.push_back(Character);
			break;
		}
	}

	if (Current.size() != 0)
	{
		Out.push_back(Current);
	}

	return Out;
}

std::size_t Utils::FileSize(std::string FileName)
{
	std::ifstream _temp(FileName, std::ios::in | std::ios::ate);
	std::size_t iFSize = _temp.tellg();
	_temp.close();

	return iFSize;
}