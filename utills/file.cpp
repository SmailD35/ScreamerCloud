//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include "file.h"

using namespace std;

File::File()
= default;

File::~File()
= default;

string File::GetHash()
{
	return string(4,'0');
}

int File::GetSize()
{
	return 0;
}

FILE* File::GetFile()
{
	return nullptr;
}

string File::GetPath()
{
	return move(_path);
}

char* File::GetNextChunk()
{
	_chunksReaded++;
	return nullptr;
}

void File::ResetChunks()
{

}

int File::GetProgress()
{
	if (_chunksCount == 0) return 0;
	return int(float(_chunksReaded) / float(_chunksCount) * 100);
}

int File::CalculateHash()
{
	return 0;
}

int File::CalculateSize()
{
	return 0;
}