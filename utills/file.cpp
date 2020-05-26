//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include "file.h"

using namespace std;

string File::GetHash()
{
	return "";
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

size_t File::GetChunksCount()
{
	return _chunksCount;
}

OutFile::OutFile(size_t size, string fileDirectory, string fileName)
{
	_size = size;
	_path = fileDirectory + '/' + fileName;
	_chunksCount = ceil(float(_size) / chunkSize);

	_file.open(_path,  std::ofstream::out | std::ofstream::app | std::ofstream::binary);
}

OutFile::~OutFile()
{
	_file.close();
}

void OutFile::SetNextChunk(std::array<char, chunkSize> buf)
{
	if (_chunksCurrent >= _chunksCount) return;

	_file.write(&buf[0], chunkSize);
	_chunksCurrent++;
}

size_t OutFile::GetSize()
{
	return _size;
}

InFile::InFile(std::string filePath)
{
	_path = std::move(filePath);
	_size = GetSize();
	_chunksCount = ceil(float(_size) / chunkSize);
	_file.open(_path,  std::ifstream::in | std::ifstream::binary);
}

InFile::~InFile()
{
	_file.close();
}

std::array<char, chunkSize> InFile::GetNextChunk()
{
	std::array<char, chunkSize> buf;
	buf.fill('\0');
	_file.read(&buf[0], chunkSize);
	_chunksCurrent++;
	return buf;
}

size_t InFile::GetSize()
{
	if (_size > 0) return _size;

	struct stat stat_buf{};
	int rc = stat(_path.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}