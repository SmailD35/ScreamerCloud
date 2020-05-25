//
// Created by Aleksandr Dergachev on 23.05.2020.
//
#include "file.h"
#include <iostream>

int main()
{
	OutFile* outFile = new OutFile(4, "azaz.txt");
	outFile->SetNextChunk("vasya petya katya");
	delete outFile;

	InFile inFile("azaz.txt", 16);
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
}