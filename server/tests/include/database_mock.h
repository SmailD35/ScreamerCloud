#ifndef SERVER_DATABASE_MOCK_H
#define SERVER_DATABASE_MOCK_H

#include "gmock/gmock.h"

class DatabaseManager
{
public:
	virtual string GetUserDir();
    virtual int Register(string login, string password);
    virtual int Authorize(string login, string password);
    virtual int DeleteUser(string login, string password);
    virtual int UploadFile(string file_name, string dir_name);
    virtual FILE* DownloadFile();
    virtual int DeleteFile();
    virtual string GetFileList(string dir_name);
};

class DatabaseManagerMock : public DatabaseManager
{
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