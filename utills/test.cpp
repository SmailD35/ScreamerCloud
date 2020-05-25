//
// Created by Aleksandr Dergachev on 23.05.2020.
//
#include "file.h"
#include <iostream>

int main()
{
	auto* outFile = new OutFile(17, "azaz.txt");
	std::string buf("vasya petya katya");
	outFile->SetNextChunk(buf);
	delete outFile;

	InFile inFile("azaz.txt", 4);
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
	std::cout << inFile.GetNextChunk();
}