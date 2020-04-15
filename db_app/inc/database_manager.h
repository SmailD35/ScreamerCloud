//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_DATABASE_MANAGER_H
#define DB_APP_DATABASE_MANAGER_H


#include <string>
#include "files_database_manager.h"
#include "users_database_manager.h"

class DatabaseManager{
private:
    UsersDatabaseManager _usersDatabaseManager;

    FilesDatabaseManager _filesDatabaseManager;

    int _userID;

    string _userDirectory;
public:
    DatabaseManager();
    ~DatabaseManager() = default;

    string GetUserDir();

    int Register(string const& login, string const& password);

    int Authorize(string const& login, string const& password);

    int DeleteUser(string const& login, string const& password);

    int Upload(string const &file_name, string const &dir_name, string const &hash_sum) ;

    FILE * Download(string const& file_name, string const& dir_name);

    int DeleteFile(string const& file_name, string const& dir_name);

    vector <string> GetFileList(string const& dir_name);
};


#endif //DB_APP_DATABASE_MANAGER_H
