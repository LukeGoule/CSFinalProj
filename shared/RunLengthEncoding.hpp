#pragma once

#ifndef RLE_HPP
#define RLE_HPP

#include <vector>
#include <iostream>
#include <fstream>

namespace Utils
{
	namespace RLE
	{
		// Single character RLE pack
		// sizeof(RLEPack) = 8 + 16 = 24 bytes
		// If we, for example, had a string of 8 character that is just "0000000\0",
		// 8*8 = 64 bytes for this single string.
		// We can run a packing function on it and downsize the data from 64 bytes to 24.
		// This is a reduction in size of {100 - (24/64)*100 =} 62.5 percent.
		struct RLEPack
		{
			// cCharacter is the character packed down
			char cCharacter;

			// iCount is the number of cCharacter(s) in a row.
			// A short is 16 bytes so the maximum is 65336, 
			// I highly doubt there will be something like 65k zeros in a row.
			unsigned short iCount;
		};

		void Dump(std::vector<RLEPack> vecOutput);

		std::vector<RLEPack> Pack(const char* szBytes, std::size_t nBytes);

		// exports to another extension i.e. program.bin becomes compressed.program.bin
		void PackFile(std::string FileName);
		void PackFile(std::string FileName, const char* BufferedData, std::size_t nBytes);

		std::string UnpackFile(std::string FileName);

		std::string Unpack(std::vector<RLEPack> vecPacked);		
	}
}

#endif