#include "RunLengthEncoding.hpp"
#include "Utils2.hpp"

void Utils::RLE::Dump(std::vector<Utils::RLE::RLEPack> vecOutput)
{
	for (auto Pack : vecOutput)
	{
		std::cout << Pack.cCharacter << "   count = " << Pack.iCount << std::endl;
	}
}

[[nodiscard]] std::vector<Utils::RLE::RLEPack> Utils::RLE::Pack(const char* szBytes, std::size_t nBytes)
{
	std::vector<Utils::RLE::RLEPack> Out;

	char cCurrent = '\0';
	unsigned short iCurrentCount = 1;
	bool bFirstComplete = false;

	for (std::size_t i = 0; i < nBytes; i++)
	{
		if (i == 0)
		{
			cCurrent = szBytes[i];
			continue;
		}

		if (szBytes[i] != cCurrent)
		{
			Out.push_back({ cCurrent, iCurrentCount });
			cCurrent = szBytes[i];
			iCurrentCount = 1;
		}
		else
		{
			iCurrentCount++;
		}
	}

	if (iCurrentCount != 0)
	{
		Out.push_back({ cCurrent, iCurrentCount });
	}

	return Out;
}

void Utils::RLE::PackFile(std::string FileName)
{
	std::ifstream In(FileName, std::ios::in | std::ios::binary);

	auto iFSize = Utils::FileSize(FileName);
	char* buf = (char*)malloc(iFSize);

	In.read(buf, iFSize);
	In.close();

	std::vector<RLEPack> OutData = Pack(buf, iFSize);

	free(buf);

	std::ofstream(FileName).close(); // Make the file if it doesn't already exist.
	std::ofstream Out(FileName, std::ios::out | std::ios::binary);

	for (auto Pack : OutData)
	{
		Out.write((const char*)&Pack, sizeof(RLEPack));
	}

	Out.close();
}

void Utils::RLE::PackFile(std::string FileName, const char* BufferedData, std::size_t nBytes)
{
	std::vector<RLEPack> OutData = Pack(BufferedData, nBytes);

	std::ofstream(FileName).close(); // Make the file if it doesn't already exist.
	std::ofstream Out(FileName, std::ios::out | std::ios::binary);

	for (auto Pack : OutData)
	{
		Out.write((const char*)&Pack, sizeof(RLEPack));
	}

	Out.close();
}

[[nodiscard]] std::string Utils::RLE::UnpackFile(std::string FileName)
{
	auto iFSize = Utils::FileSize(FileName);

	if (iFSize % sizeof(RLEPack) != 0)
	{
		std::cout << "fsize % sizeof(RLEPack) != 0" << std::endl;
		return {};
	}
	
	std::vector<RLEPack> InPacked = std::vector<RLEPack>(iFSize);

	std::ifstream In(FileName, std::ios::in | std::ios::binary);

	std::size_t index = 0;
	for (int i = 0; i < iFSize; i += sizeof(RLEPack), index++)
	{
		RLEPack* pNew = new RLEPack;

		In.read(reinterpret_cast<char*>(pNew), sizeof(RLEPack));

		InPacked[index] = *pNew;
	}

	return Utils::RLE::Unpack(InPacked);
}

[[nodiscard]] std::string Utils::RLE::Unpack(std::vector<Utils::RLE::RLEPack> vecPacked)
{
	std::string Output;

	for (auto Pack : vecPacked)
	{
		for (auto i = 0; i < Pack.iCount; i++)
		{
			Output.push_back(Pack.cCharacter);
		}
	}

	return Output;
}