#ifndef UTILLS_LIBRARY_H
#define UTILLS_LIBRARY_H

#include <string>
#include <fstream>
#include <cmath>

class File
{
 public:
	virtual ~File() = default;
	std::string GetHash();
	void ResetChunks();
	int GetProgress();
	int CalculateHash();
	virtual size_t GetSize() = 0;

 protected:
	std::string _hashSum;
	size_t _chunkSize = 1;
	size_t _size = 0;
	size_t _chunksCurrent = 0;
	size_t _chunksCount = 0;
	std::string _path;
};

class OutFile : public File
{
 public:
	OutFile(size_t size, std::string fileDirectory, std::string fileName, size_t chunkSize = 1024);
	~OutFile() override;
	void SetNextChunk(std::string buf);
	size_t GetSize() override;

 private:
	std::ofstream _file;
};

class InFile : public File
{
 public:
	explicit InFile(std::string filePath, size_t chunkSize = 1024);
	~InFile() override;
	std::string GetNextChunk();
	size_t GetSize() override;

 private:
	std::ifstream _file;
};

#endif //UTILLS_LIBRARY_H