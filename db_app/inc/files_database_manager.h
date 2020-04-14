//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_FILES_DATABASE_MANAGER_H
#define DB_APP_FILES_DATABASE_MANAGER_H


#include <string>

class FilesDatabaseManager{
private:
    int _userID;

    std::string _userDirectory;
public:
    FilesDatabaseManager() {
        _userID = -1;
        _userDirectory = "";
    }
    ~FilesDatabaseManager() = default;


    void SetUserID(int userID) {
        _userID = userID;
    }

    void SetUserDirectory(std::string userDirectory) {
        _userDirectory = userDirectory;
    }

    int UploadFile(std::string const& file_name, std::string const& dir_name, int hash_sum) {}

    FILE * DownloadFile(std::string const& file_name, std::string const& dir_name) {}

    int DeleteFile(std::string const& file_name, std::string const& dir_name ) {}

    char * GetFileList(std::string const& file_name, std::string const& dir_name) {}
};


#endif //DB_APP_FILES_DATABASE_MANAGER_H
