#pragma once

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>

namespace Utils
{
	/*
	Convert an object into a binary file.
	*/
	template<typename T>
	inline void ExportObjectToFile(T Object, std::string FileName)
	{
		std::ofstream(FileName).close(); // Create a new file if it doesn't already exist.
		std::ofstream wf(FileName, std::ios::out | std::ios::binary);

		char* buffer[1024];
		memcpy(buffer, &Object, sizeof(Object));

		wf.write((const char*)buffer, sizeof(Object));
		wf.close();
	}

	/*
	Import data from a binary file into an object.
	Could be used as an attack vector so be warned when loading secure things.
	*/
	template<typename T>
	inline T* ImportObjectFromFile(std::string FileName)
	{
		std::ifstream _temp(FileName, std::ios::in | std::ios::ate);
		std::size_t fsize = _temp.tellg();
		_temp.close();

		T* NewObject = (T*)malloc(sizeof(T));

		std::ifstream in(FileName, std::ios::in | std::ios::binary);

		char* buffer = (char*)malloc(fsize);

		in.read(buffer, fsize);

		memcpy((void*)NewObject, buffer, fsize);

		return NewObject;
	}

	std::size_t FileSize(std::string FileName);

	std::vector<std::string> Tokenise(std::string str);
};

#endif