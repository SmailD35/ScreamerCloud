//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include <sys/stat.h>
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

OutFile::OutFile(size_t size, string fileDirectory, string fileName, size_t chunkSize)
{
	_size = size;
	_path = fileDirectory + '/' + fileName;
	_chunkSize = chunkSize;
	_chunksCount = ceil(float(_size) / chunkSize);

	_file.open(_path,  std::ofstream::out | std::ofstream::app | std::ofstream::binary);
}

OutFile::~OutFile()
{
	_file.close();
}

void OutFile::SetNextChunk(string buf)
{
	if (_chunksCurrent >= _chunksCount) return;
	if (buf.size() > _chunkSize) return;
	_file.write(buf.c_str(), buf.size());
	_chunksCurrent++;
}

size_t OutFile::GetSize()
{
	return _size;
}

InFile::InFile(std::string filePath, size_t chunkSize)
{
	_path = std::move(filePath);
	_size = GetSize();
	_chunkSize = chunkSize;
	_chunksCount = ceil(float(_size) / chunkSize);
	_file.open(_path,  std::ifstream::in | std::ifstream::binary);
}

InFile::~InFile()
{
	_file.close();
}

string InFile::GetNextChunk()
{
	if (_chunksCurrent >= _chunksCount) return "";

	if (_chunkSize > _size)
	{
		char buf[_size];
		_file.read(buf, _size);
		string result(buf, _size);
		_chunksCurrent++;
		return result;
	}

	if (_chunksCurrent == _chunksCount)
	{
		size_t size = _size - _chunkSize * _chunksCurrent;
		char buf[size];
		_file.read(buf, size);
		string result(buf, size);
		_chunksCurrent++;
		return result;
	}
	char buf[_chunkSize];
	_file.read(buf, _chunkSize);
	string result(buf, _chunkSize);
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