//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include "file.h"

using namespace std;

File::File()
{

}

File::~File()
{

}

string File::GetHash()
{
	return string();
}

int File::GetSize()
{
	return 0;
}

FILE* File::GetFile()
{
	return nullptr;
}

char* File::GetNextChunk()
{
	return nullptr;
}

void File::ResetChunks()
{

}

int File::GetProgress()
{
	return 0;
}

void File::SetFile(string file_name)
{

}

int File::CalculateHash()
{
	return 0;
}

int File::CalculateSize()
{
	return 0;
}