//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_FILES_DATABASE_MANAGER_H
#define DB_APP_FILES_DATABASE_MANAGER_H

#include "database_wrapper.h"


class FilesDatabaseManager{
private:
    int _userID;

    std::string _userDirectory;

    bool CheckExistingFile(const std::string &file_name, const std::string &dir_name);

    bool AddFileRecord(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum);

    bool DeleteFileRecord(const std::string &file_name, const std::string &dir_name);

    std::shared_ptr<FILE> GetFilePtr(const std::string &file_name, const std::string &dir_name) throw (DBExceptions);

    DatabaseWrapper _databaseConnection;
public:
    FilesDatabaseManager();
    ~FilesDatabaseManager() = default;

    void SetUserID(int userID);

    void SetUserDirectory(const std::string userDirectory);

    bool UploadFile(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum);

    std::shared_ptr<FILE> DownloadFile(const std::string &file_name, const std::string &dir_name) throw(DBExceptions);

    bool DeleteFile(const std::string &file_name, const std::string &dir_name);

    std::vector <std::string> GetFileList(const std::string &dir_name);
};


#endif //DB_APP_FILES_DATABASE_MANAGER_H
