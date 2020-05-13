//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/database_manager.h"
using namespace std;

DatabaseManager::DatabaseManager() {
    _usersDatabaseManager = UsersDatabaseManager();
    _filesDatabaseManager = FilesDatabaseManager();
    _userID = NIL;
}

string DatabaseManager::GetUserDir() {
    return _userDirectory;
}


bool DatabaseManager::Register(const string &login, const string &password) {
    int userID =  _usersDatabaseManager.RegisterUser(login, password);
    if (userID == FAIL)
        return false;
    else {
        _userID = userID;
        _userDirectory = to_string(_userID);
        _filesDatabaseManager.SetUserID(_userID);
        _filesDatabaseManager.SetUserDirectory(_userDirectory);
        return true;
    }
}


bool DatabaseManager::Authorize(const string &login, const string &password) {
    int userID = _usersDatabaseManager.AuthorizeUser(login, password);

    if (userID == FAIL)
        return false;
    //в случае, если пользователь уже был зарегистрирован до текущего сессии работы с программой, необходимо достать его ID и запомнить его
    if (_userID == NIL) {
        _userID = userID;
        _userDirectory = to_string(_userID);
        _filesDatabaseManager.SetUserID(_userID);
        _filesDatabaseManager.SetUserDirectory(_userDirectory);
    }
    return true;
}

bool DatabaseManager::DeleteUser(const string &login, const string &password) {
    return _usersDatabaseManager.DeleteUser(login, password);
}

bool DatabaseManager::Upload(const string &file_name, const string &dir_name, const string &hash_sum) {
    return _filesDatabaseManager.UploadFile(file_name, dir_name, hash_sum);
}

FILE* DatabaseManager::Download(string const& file_name, string const& dir_name) {
    ////пытаемся перехватить исключение, если получилось - пробрасываем дальше серверу
    ////иначе возвращаем указатель на файл
    try {
        FILE *file = _filesDatabaseManager.DownloadFile(file_name, dir_name);
        return file;
    }
    catch (DBExceptions &exceptions) {
        exceptions.m_what();
    }
    return nullptr;
}


bool DatabaseManager::DeleteFile(const string &file_name, const string &dir_name) {
    return _filesDatabaseManager.DeleteFile(file_name, dir_name);
}

vector <string> DatabaseManager::GetFileList(string const& dir_name) {
   ////////////////////////////////////////////////////////
    return _filesDatabaseManager.GetFileList(dir_name);
}

