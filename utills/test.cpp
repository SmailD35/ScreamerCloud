//
// Created by Aleksandr Dergachev on 23.05.2020.
//
#include "file.h"
#include <iostream>

int main()
{
	auto* outFile = new OutFile(12530671, "azaz", "video.mkv");
	InFile inFile("/home/egor/Видео/angl-2020-04-27_23.03.52.mkv");

	std::array<char, chunkSize> buffer;
	buffer.fill('\0');
	
	size_t chunksCount = inFile.GetChunksCount();
	for (size_t i = 0; i < chunksCount; i++)
	{
		buffer = inFile.GetNextChunk();
		outFile->SetNextChunk(buffer);
	}

	delete outFile;
}