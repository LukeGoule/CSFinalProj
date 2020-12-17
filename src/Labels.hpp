#pragma once

#ifndef LABELS_HPP
#define LABELS_HPP

#include <unordered_map>
#include <string>

template<typename LabelAddressWidth>
class LabelManager
{
public:

	std::unordered_map<std::string, LabelAddressWidth> m_LabelMap;

	LabelManager()
	{
	}

	~LabelManager()
	{
	}

	void InsertLabel(std::string Identifier, LabelAddressWidth Address)
	{
		this->m_LabelMap[Identifier] = Address;
	}

	void Reset()
	{
		this->m_LabelMap.clear(); // Untested, currently.
	}

};

#endif