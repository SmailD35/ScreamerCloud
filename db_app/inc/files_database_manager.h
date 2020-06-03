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

    std::string _path_users_storage;

    void SetPathToUsersStorage();

	std::shared_ptr<InFile> GetFilePtr(int file_ID);

    DatabaseWrapper _databaseConnection;
public:
    FilesDatabaseManager();

    ~FilesDatabaseManager() = default;

    std::string  GetPathToUsersStorage();

    void SetUserID(int userID);

    void SetUserDirectory(const std::string userDirectory);

    bool CheckExistingFile(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error);

    bool UploadFile(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum,
                    DbErrorCodes &error);

	std::shared_ptr<InFile>
    DownloadFile(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error);

    bool DeleteFile(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error);

    std::map<std::string, std::string> GetFileList(const std::string &dir_name, DbErrorCodes &error);

    std::string GetPublicLink(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error);

    std::shared_ptr<InFile> DownloadFileByLink(const std::string &link, DbErrorCodes &error);
};


#endif //DB_APP_FILES_DATABASE_MANAGER_H
