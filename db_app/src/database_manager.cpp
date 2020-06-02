//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/database_manager.h"
using namespace std;
namespace fs = boost::filesystem;

DatabaseManager::DatabaseManager() {
    _usersDatabaseManager = UsersDatabaseManager();
    _filesDatabaseManager = FilesDatabaseManager();
    _userID = NIL;
    InitLogging();
}

string DatabaseManager::GetUserDir() {
    return _userDirectory;
}


bool DatabaseManager::Register(const std::string &login, const std::string &password, DbErrorCodes &error) {
    int userID = _usersDatabaseManager.RegisterUser(login, password, error);
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


bool DatabaseManager::Authorize(const std::string &login, const std::string &password, DbErrorCodes &error) {
    int userID = _usersDatabaseManager.AuthorizeUser(login, password, error);

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

bool DatabaseManager::DeleteUser(const string &login, const string &password, DbErrorCodes &error) {
    return _usersDatabaseManager.DeleteUser(login, password, error);
}

bool DatabaseManager::Upload(const string &file_name, const string &dir_name, const string &hash_sum, DbErrorCodes &error) {
    return _filesDatabaseManager.UploadFile(file_name, dir_name, hash_sum, error);
}

shared_ptr<InFile> DatabaseManager::Download(const string &file_name, const string &dir_name, DbErrorCodes &error) {
	shared_ptr<InFile> file = _filesDatabaseManager.DownloadFile(file_name, dir_name, error);
	return file;
}


bool DatabaseManager::DeleteFile(const string &file_name, const string &dir_name, DbErrorCodes &error) {
    return _filesDatabaseManager.DeleteFile(file_name, dir_name, error);
}

map <string, string> DatabaseManager::GetFileList(string const& dir_name, DbErrorCodes &error) {
    return _filesDatabaseManager.GetFileList(dir_name, error);
}

unsigned long DatabaseManager::CheckAvailableSpace() {
    fs::space_info info = fs::space(_filesDatabaseManager.GetPathToUsersStorage());
    return info.available;
}

std::string DatabaseManager::GetPublicLink(const string &file_name, const string &dir_name, DbErrorCodes &error) {
    return _filesDatabaseManager.GetPublicLink(file_name, dir_name, error);
}

std::shared_ptr<InFile> DatabaseManager::DownloadByLink(const string &link, DbErrorCodes &error) {
    shared_ptr<InFile> file = _filesDatabaseManager.DownloadFileByLink(link, error);
    return file;
}
 
