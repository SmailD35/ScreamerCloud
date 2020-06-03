//
// Created by Aleksandr Dergachev on 23.05.2020.
//
#include "file.h"
#include <iostream>

int main()
{
	auto* outFile = new OutFile(12530671, "azaz", "photo.mkv");
	InFile inFile("/Users/smaild/Desktop/DSC_0150.NEF");
	inFile.GetHash();

	std::array<char, chunkSize> buffer;
	buffer.fill('\0');
	
	size_t chunksCount = inFile.GetChunksCount();
	for (size_t i = 0; i < chunksCount; i++)
	{
		buffer = inFile.GetNextChunk();
		outFile->SetNextChunk(buffer);
	}
	outFile->GetHash();

	delete outFile;
}