#ifndef NETWORK_TEST_NETWORK_CLIENT_H
#define NETWORK_TEST_NETWORK_CLIENT_H

#include "gmock/gmock.h"
#include <string>
#include "gtest/gtest.h"
#include <cstring>

#define UPLOAD_SRV 		"1"
#define DOWNLOAD_SRV 	"2"
#define DELETE_SRV 		"3"
#define LIST_SRV 		"4"
#define AUTH_SRV 		"5"

using namespace std;


class File
{
public:
	//virtual ~File() {};
	virtual string GetHash() = 0;
	virtual int GetSize()  = 0;
	virtual FILE* GetFile() = 0;
};


class FileMock : public File
{
public:
	MOCK_METHOD(string, GetHash, (), (override));
	MOCK_METHOD(int, GetSize, (), (override));
	MOCK_METHOD(FILE *, GetFile, (), (override));
};


#endif //NETWORK_TEST_NETWORK_CLIENT_H
