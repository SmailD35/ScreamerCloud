//
// Created by Aleksandr Dergachev on 23.05.2020.
//
#include "file.h"
#include <iostream>

int main()
{
	InFile inFile("/home/egor/Загрузки/uml.png");
	auto* outFile = new OutFile(inFile.GetSize(), "azaz", "photo.png");
	std::cout << inFile.GetHash() << std::endl;

	std::array<char, chunkSize> buffer;
	buffer.fill('\0');
	
	size_t chunksCount = inFile.GetChunksCount();
	for (size_t i = 0; i <= chunksCount; i++)
	{
		buffer = inFile.GetNextChunk();
		outFile->SetNextChunk(buffer);
	}
	std::cout << outFile->GetHash();

	delete outFile;
}