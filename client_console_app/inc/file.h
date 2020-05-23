//
// Created by Aleksandr Dergachev on 11.04.2020.
//

#ifndef CLIENT_CONSOLE_APP__FILE_H_
#define CLIENT_CONSOLE_APP__FILE_H_

#include <cstdio>
#include <string>

class File
{
 public:
	File();
	~File();
	std::string GetHash();
	int GetSize();
	FILE* GetFile();
	char* GetNextChunk();
	void ResetChunks();
	int GetProgress();
	void SetFile(std::string file_name);
 private:
	int CalculateHash();
	int CalculateSize();
 private:
	std::string _hashSum;
	int _size = 0;
	int _chunksReaded = 0;
	int _chunksCount = 0;
	std::string _path;
	FILE* file = nullptr;
};

#endif //CLIENT_CONSOLE_APP__FILE_H_