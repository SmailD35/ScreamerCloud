//
// Created by Aleksandr Dergachev on 11.04.2020.
//

#ifndef CLIENT_CONSOLE_APP__FILE_H_
#define CLIENT_CONSOLE_APP__FILE_H_

#include <cstdio>
#include <string>
using namespace std;

class File
{
 public:
	File();
	~File();
	string GetHash();
	int GetSize();
	FILE* GetFile();
 private:
	string _hashSum;
	int _size = 0;
	string _path;
	FILE* file = nullptr;
	int CalculateHash();
	int CalculateSize();
};

#endif //CLIENT_CONSOLE_APP__FILE_H_