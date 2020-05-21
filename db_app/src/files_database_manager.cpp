//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/files_database_manager.h"

using namespace std;

FilesDatabaseManager::FilesDatabaseManager() {
    _userID = NIL;
    _userDirectory = "";
    _databaseConnection = DatabaseWrapper();
}

void FilesDatabaseManager::SetUserID(int userID) {
    _userID = userID;
    _databaseConnection.SetUserID(userID);
}

void FilesDatabaseManager::SetUserDirectory(string userDirectory) {
    _userDirectory = std::move(userDirectory);
}

///////////////////////////////////////////////////////////////////////////////
std::shared_ptr<FILE> FilesDatabaseManager::GetFilePtr(int file_ID) {
    ////если указатель на файл не может быть взят, throw
    return nullptr;
}

bool FilesDatabaseManager::UploadFile(const string &file_name, const string &dir_name, const string &hash_sum) {
    try {
        _databaseConnection.AddFileRecord(file_name, dir_name, hash_sum);
        return true;
        ///////////////////////////////////////////////////////////////////////////
        ////переименовать существующий файл
    }
    catch(exception &exc) {
        cout << exc.what() << endl; //////логи
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<FILE> FilesDatabaseManager::DownloadFile(string const& file_name, string const& dir_name) {
    int fileID;
    try {
        fileID = _databaseConnection.CheckFileID(file_name, dir_name);
    }
    catch (exception &exc) {
        cout << exc.what() << endl;
        return nullptr;
    }

    shared_ptr<FILE> file(GetFilePtr(fileID));
    return file;
}

bool FilesDatabaseManager::DeleteFile(const string &file_name, const string &dir_name) {
    int fileID;
    try {
         fileID = _databaseConnection.CheckFileID(file_name, dir_name);
         if (fileID == FAIL)
             return false;
    }
    catch (exception &exc) {
        cout << exc.what() << endl; ////////log
        return false;
    }

    try {
        _databaseConnection.DeleteFileRecord(fileID);
        return true;
    }
    catch (exception &exc) {
        cout << exc.what() << endl; ////////log
        return false;
    }
}

///нерекурсивный вывод файлов соответствующей директории
map<string, string> FilesDatabaseManager::GetFileList(string const& dir_name) {
    map<string, string> file_list;
    try {
        file_list = _databaseConnection.GetFileList(dir_name);
    }
    catch (exception &exc) {
        cout << exc.what() << endl; ////логи
    }
    return file_list;
}


void FilesDatabaseManager::GetPathToUsersStorage() {
    _path_users_storage = "../users_files/";
    ////считываем из конфига путь к хранящимся файлам пользователей
}

bool FilesDatabaseManager::CheckExistingFile(const std::string &file_name, const std::string &dir_name) {
    return _databaseConnection.CheckFileID(file_name, dir_name) != 0;
}
