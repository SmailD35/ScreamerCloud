#ifndef UTILLS_LIBRARY_H
#define UTILLS_LIBRARY_H

#include <string>
#include <fstream>
#include <cmath>
#include <array>
#include <iostream>
#include <sys/stat.h>
#include <openssl/md5.h>
#include <iomanip>

const size_t chunkSize = 1024;

class File
{
 public:
	virtual ~File() = default;
	size_t GetChunksCount();
	int GetProgress();
	virtual size_t GetSize() = 0;
	virtual std::string GetHash() = 0;

protected:
	std::string GetHashInternal();

protected:
	std::string _hashSum;
	size_t _size = 0;
	size_t _chunksCurrent = 1;
	size_t _chunksCount = 0;
	std::string _path;
};

class OutFile : public File
{
 public:
	OutFile(size_t size, std::string fileDirectory, std::string fileName);
	~OutFile() override;
	void SetNextChunk(std::array<char, chunkSize> buf);
	size_t GetSize() override;
	std::string GetHash() override;

 private:
	std::ofstream _file;
};

class InFile : public File
{
 public:
	explicit InFile(std::string filePath);
	~InFile() override;
	std::array<char, chunkSize> GetNextChunk();
	size_t GetSize() override;
	std::string GetHash() override;

private:
	std::ifstream _file;
};

#endif //UTILLS_LIBRARY_H