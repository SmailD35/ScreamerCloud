#ifndef UTILLS_LIBRARY_H
#define UTILLS_LIBRARY_H

#include <string>
#include <fstream>
#include <cmath>

class File
{
 public:
	virtual ~File() = default;;
	std::string GetHash();
	void ResetChunks();
	int GetProgress();
	int CalculateHash();
	int GetSize();

 protected:
	std::string _hashSum;
	int _chunkSize = 1;
	int _size = 0;
	int _chunksCurrent = 0;
	int _chunksCount = 0;
	std::string _path;
};

class OutFile : public File
{
 public:
	OutFile(int size, std::string filePath, int chunkSize = 1024);
	~OutFile() override;
	void SetNextChunk(std::string buf);

 private:
	std::ofstream _file;
};

class InFile : public File
{
 public:
	InFile(int size, std::string filePath, int chunkSize = 1024);
	~InFile() override;
	std::string GetNextChunk();

 private:
	std::ifstream _file;
};

#endif //UTILLS_LIBRARY_H