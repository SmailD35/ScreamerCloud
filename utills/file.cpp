//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include "file.h"

using namespace std;

string File::GetHash()
{
	return string(4,'0');
}

void File::ResetChunks()
{
	_chunksCurrent = 0;
}

int File::GetProgress()
{
	if (_chunksCount == 0) return 0;
	return int(float(_chunksCurrent) / float(_chunksCount) * 100);
}

int File::CalculateHash()
{
	return 0;
}

OutFile::OutFile(int size, std::string filePath, int chunkSize)
{
	_size = size;
	_path = std::move(filePath);
	_chunkSize = chunkSize;
	_file.open(_path,  std::ofstream::out | std::ofstream::app);
}

OutFile::~OutFile()
{
	_file.close();
}

void OutFile::SetNextChunk(string buf)
{
	_file << buf;
	_chunksCurrent++;
}

InFile::InFile(int size, std::string filePath, int chunkSize)
{
	_size = size;
	_path = std::move(filePath);
	_chunkSize = chunkSize;
	_file.open(_path,  std::ifstream::in);
}

InFile::~InFile()
{
	_file.close();
}

string InFile::GetNextChunk()
{
	string buf;
	_file >> buf;
	_chunksCurrent++;
	return std::move(buf);
}