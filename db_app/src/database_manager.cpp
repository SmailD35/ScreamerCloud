//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/database_manager.h"

DatabaseManager::DatabaseManager() {
    _usersDatabaseManager = UsersDatabaseManager();
    _filesDatabaseManager = FilesDatabaseManager();
}

string DatabaseManager::GetUserDir() {
    return _userDirectory;
}

int DatabaseManager::Register(string const& login, string const& password) {
    _userID = _usersDatabaseManager.RegisterUser(login, password);
    if (_userID == -1)
        return 1;
    _userDirectory = std::to_string(_userID);
    _filesDatabaseManager.SetUserID(_userID);
    _filesDatabaseManager.SetUserDirectory(_userDirectory);
    return 0;
}

int DatabaseManager::Authorize(string const& login, string const& password) {
    int userID = _usersDatabaseManager.AuthorizeUser(login, password);

    if (userID == -1)
        return 1;
    //в случае, если пользователь уже был зарегестрирован до текущего сессии работы с программой, необходимо достать его ID и запомнить его
    if (!_userID) {
        _userID = userID;
        _userDirectory = to_string(_userID);
        _filesDatabaseManager.SetUserID(_userID);
        _filesDatabaseManager.SetUserDirectory(_userDirectory);
    }
    return 0;
}

int DatabaseManager::DeleteUser(string const& login, string const& password) {
    if (_usersDatabaseManager.DeleteUser(login, password))
        return 1;
    return 0;
}

int DatabaseManager::Upload(string const &file_name, string const &dir_name, string const &hash_sum) {
    _filesDatabaseManager.UploadFile(file_name, dir_name, hash_sum);
}

FILE* DatabaseManager::Download(string const& file_name, string const& dir_name) {
    _filesDatabaseManager.DownloadFile(file_name, dir_name);
}

int DatabaseManager::DeleteFile(string const& file_name, string const& dir_name) {
    _filesDatabaseManager.DeleteFile(file_name, dir_name);
}

vector <string> DatabaseManager::GetFileList(string const& dir_name) {
    _filesDatabaseManager.GetFileList(dir_name);
}

