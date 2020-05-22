//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_DATABASE_MANAGER_H
#define DB_APP_DATABASE_MANAGER_H

#include "files_database_manager.h"
#include "users_database_manager.h"

class DatabaseManager{
private:
    UsersDatabaseManager _usersDatabaseManager;

    FilesDatabaseManager _filesDatabaseManager;

    int _userID;

    std::string _userDirectory;
public:
    DatabaseManager();

    ~DatabaseManager() = default;

    std::string GetUserDir();

    unsigned long CheckAvailableSpace();

    bool Register(const std::string &login, const std::string &password);

    bool Authorize(const std::string &login, const std::string &password);

    bool DeleteUser(const std::string &login, const std::string &password);

    bool Upload(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum) ;

    std::shared_ptr<FILE> Download(const std::string &file_name, const std::string &dir_name);

    bool DeleteFile(const std::string &file_name, const std::string &dir_name);

    std::map <std::string, std::string> GetFileList(const std::string &dir_name);
};


#endif //DB_APP_DATABASE_MANAGER_H
