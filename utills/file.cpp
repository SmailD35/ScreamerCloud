//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include <sys/stat.h>
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

OutFile::OutFile(size_t size, std::string filePath, size_t chunkSize)
{
	_size = size;
	_path = std::move(filePath);
	_chunkSize = chunkSize;
	_chunksCount = ceil(_size / chunkSize);
	_file.open(_path,  std::ofstream::out | std::ofstream::app);
}

OutFile::~OutFile()
{
	_file.close();
}

void OutFile::SetNextChunk(string buf)
{
	if (_chunksCurrent >= _chunksCount) return;
	_file << buf;
	_chunksCurrent++;
}

InFile::InFile(std::string filePath, size_t chunkSize)
{
	_path = std::move(filePath);
	_size = GetSize();
	_chunkSize = chunkSize;
	_chunksCount = ceil(float(_size) / chunkSize);
	_file.open(_path,  std::ifstream::in);
}

InFile::~InFile()
{
	_file.close();
}

string InFile::GetNextChunk()
{
	if (_chunksCurrent >= _chunksCount) return "";
	string buf;
	_file >> buf;
	_chunksCurrent++;
	return std::move(buf);
}

size_t InFile::GetSize()
{
	if (_size > 0) return _size;

	struct stat stat_buf{};
	int rc = stat(_path.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}