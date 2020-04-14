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

    std::string _userDirectory;
public:
    DatabaseManager() {
        _usersDatabaseManager = UsersDatabaseManager();
        _filesDatabaseManager = FilesDatabaseManager();
    }

    ~DatabaseManager() = default;

    std::string GetUserDir() {
        return _userDirectory;
    }

    int Register(std::string const& login, std::string const& password) {
        _userID = _usersDatabaseManager.RegisterUser(login, password);
        if (_userID == -1)
            return 1;
        _userDirectory = std::to_string(_userID);
        _filesDatabaseManager.SetUserID(_userID);
        _filesDatabaseManager.SetUserDirectory(_userDirectory);
        return 0;
    }

    int Authorize(std::string const& login, std::string const& password) {
        int userID = _usersDatabaseManager.AuthorizeUser(login, password);

        if (userID == -1)
            return 1;
        //в случае, если пользователь уже был зарегестрирован до текущего сессии работы с программой
        if (!_userID) {
            _userID = userID;
            _userDirectory = std::to_string(_userID);
            _filesDatabaseManager.SetUserID(_userID);
            _filesDatabaseManager.SetUserDirectory(_userDirectory);
        }
        return 0;
    }

    int DeleteUser(std::string const& login, std::string const& password) {
        if (_usersDatabaseManager.DeleteUser(login, password))
            return 1;
        return 0;
    }

    int UploadFile(std::string const &file_name, std::string const &dir_name, int hash_sum) {
        _filesDatabaseManager.UploadFile(file_name, dir_name, hash_sum);
    }

    FILE * DownloadFile(std::string const& file_name, std::string const& dir_name) {
        _filesDatabaseManager.DownloadFile(file_name, dir_name);
    };

    int DeleteFile(std::string const& file_name, std::string const& dir_name) {
        _filesDatabaseManager.DeleteFile(file_name, dir_name);
    };

    char * GetFileList(std::string const& file_name, std::string const& dir_name) {
        _filesDatabaseManager.GetFileList(file_name, dir_name);
    };
};


#endif //DB_APP_DATABASE_MANAGER_H
