//
// Created by Aleksandr Dergachev on 23.05.2020.
//
#include "file.h"
#include <iostream>

int main()
{
	OutFile* outFile = new OutFile(4, "azaz.txt");
	outFile->SetNextChunk("azazazazazazazaz");
	delete outFile;

	InFile inFile("azaz.txt");
	std::cout << inFile.GetNextChunk();
}