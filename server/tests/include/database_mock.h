#ifndef SERVER_DATABASE_MOCK_H
#define SERVER_DATABASE_MOCK_H

class DatabaseManager
{
public:
    virtual int Register(string login, string password);
    virtual int Authorize(string login, string password);
    virtual int DeleteUser(string login, string password);
    virtual int UploadFile(string file_name, string dir_name);
    virtual FILE* DownloadFile();
    virtual int DeleteFile();
    virtual string GetFileList(string dir_name);
};

#endif //SERVER_DATABASE_MOCK_H
