#ifndef UTILLS_LIBRARY_H
#define UTILLS_LIBRARY_H

#include <cstdio>
#include <string>

class File
{
 public:
	File();
	File(FILE* file, int size, std::string filePath) :
		_file(file), _size(size), _path(std::move(filePath)), _chunkSize(1024) {};
	File(FILE* file, int size, std::string filePath, int chunkSize) :
		_file(file), _size(size), _path(std::move(filePath)), _chunkSize(chunkSize) {};
	~File();
	std::string GetHash();
	int GetSize();
	FILE* GetFile();
	std::string GetPath();
	char* GetNextChunk();
	void ResetChunks();
	int GetProgress();
 private:
	int CalculateHash();
	int CalculateSize();

 private:
	std::string _hashSum;
	int _chunkSize = 1;
	int _size = 0;
	int _chunksReaded = 0;
	int _chunksCount = 0;
	std::string _path;
	FILE* _file = nullptr;
};

#endif //UTILLS_LIBRARY_H