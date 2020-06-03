//
// Created by Aleksandr Dergachev on 20.04.2020.
//

#include "file.h"

using namespace std;

int File::GetProgress()
{
	if (_chunksCount == 0) return 0;
	return int(float(_chunksCurrent) / float(_chunksCount) * 100);
}

size_t File::GetChunksCount()
{
	return _chunksCount;
}

std::string File::GetHashInternal()
{
	MD5_CTX ctx;
	MD5_Init(&ctx);

	ifstream ifs(_path, ios::binary);

	char file_buffer[4096];
	while (ifs.read(file_buffer, sizeof(file_buffer)) || ifs.gcount()) {
		MD5_Update(&ctx, file_buffer, ifs.gcount());
	}
	unsigned char digest[MD5_DIGEST_LENGTH] = {};
	MD5_Final(digest, &ctx);

	std::stringstream result;
	for(unsigned char i : digest)
		result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

	return result.str();
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
	if (_chunksCurrent > _chunksCount) return;

	if (_chunksCurrent == _chunksCount)
	{
		size_t size = _size - (_chunksCurrent - 1) * chunkSize;
		_file.write(&buf[0], size);
		_chunksCurrent++;
		return;
	}

	_file.write(&buf[0], chunkSize);
	_chunksCurrent++;
}

size_t OutFile::GetSize()
{
	return _size;
}

std::string OutFile::GetHash()
{
	_file.close();
	string result = GetHashInternal();
	_file.open(_path,  std::ofstream::out | std::ofstream::app | std::ofstream::binary);

	return result;
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

std::string InFile::GetHash()
{
	_file.close();
	string result = GetHashInternal();
	_file.open(_path,  std::ifstream::in | std::ifstream::binary);

	return result;}
