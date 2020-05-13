#ifndef SERVER_DATABASE_MOCK_H
#define SERVER_DATABASE_MOCK_H

#include "gmock/gmock.h"

class DatabaseManager
{
public:
	virtual ~DatabaseManager() {};
	virtual string GetUserDir() = 0;
    virtual int Register(string login, string password) = 0;
    virtual int Authorize(string login, string password) = 0;
    virtual int DeleteUser(string login, string password) = 0;
    virtual int UploadFile(string file_name, string dir_name) = 0;
    virtual FILE* DownloadFile() = 0;
    virtual int DeleteFile() = 0;
    virtual string GetFileList(string dir_name) = 0;
};

class DatabaseManagerMock : public DatabaseManager
{
public:
	MOCK_METHOD(string, GetUserDir, (), (override));
	MOCK_METHOD(int, Register, (string login, string password), (override));
	MOCK_METHOD(int, Authorize, (string login, string password), (override));
	MOCK_METHOD(int, DeleteUser, (string login, string password), (override));
	MOCK_METHOD(int, UploadFile, (string file_name, string dir_name), (override));
	MOCK_METHOD(FILE*, DownloadFile, (), (override));
	MOCK_METHOD(int, DeleteFile, (), (override));
	MOCK_METHOD(string, GetFileList, (string dir_name), (override));
};

#endif //SERVER_DATABASE_MOCK_H