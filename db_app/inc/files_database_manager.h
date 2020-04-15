//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_FILES_DATABASE_MANAGER_H
#define DB_APP_FILES_DATABASE_MANAGER_H

using namespace std;

#include <string>
#include <vector>

class FilesDatabaseManager{
private:
    int _userID;

    string _userDirectory;

    int CheckExistingFile();

    int AddFileRecord();

    int DeleteFileRecord();

    FILE* GetFilePtr();
public:
    FilesDatabaseManager();
    ~FilesDatabaseManager() = default;


    void SetUserID(int userID);

    void SetUserDirectory(string userDirectory);

    int UploadFile(string const& file_name, string const& dir_name, string const& hash_sum);

    FILE * DownloadFile(string const& file_name, string const& dir_name);

    int DeleteFile(string const& file_name, string const& dir_name );

    vector <string> GetFileList(string const& dir_name);
};


#endif //DB_APP_FILES_DATABASE_MANAGER_H
