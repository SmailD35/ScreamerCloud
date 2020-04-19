//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/files_database_manager.h"

FilesDatabaseManager::FilesDatabaseManager() {
    _userID = -1;
    _userDirectory = "";
}

void FilesDatabaseManager::SetUserID(int userID) {
    _userID = userID;
}

void FilesDatabaseManager::SetUserDirectory(string userDirectory) {
    _userDirectory = userDirectory;
}

int FilesDatabaseManager::CheckExistingFile(string const &file_name, string const &dir_name) {}

int FilesDatabaseManager::AddFileRecord(string const &file_name, string const &dir_name, string const &hash_sum) {}

int FilesDatabaseManager::DeleteFileRecord(string const &file_name, string const &dir_name) {}

FILE * FilesDatabaseManager::GetFilePtr(string const &file_name, string const &dir_name) {}

int FilesDatabaseManager::UploadFile(string const& file_name, string const& dir_name, string const& hash_sum) {
    if (CheckExistingFile(file_name, dir_name))
        return 1;
    if (AddFileRecord(file_name, dir_name, hash_sum))
        return 1;
    return 0;
}

FILE* FilesDatabaseManager::DownloadFile(string const& file_name, string const& dir_name) {
    if (CheckExistingFile(file_name, dir_name))
        return nullptr;
    return GetFilePtr(file_name, dir_name);
}

int FilesDatabaseManager::DeleteFile(string const& file_name, string const& dir_name ) {
    if (CheckExistingFile(file_name, dir_name))
        return 1;
    if (DeleteFileRecord(file_name, dir_name))
        return 1;
    return 0;
}

vector <string> FilesDatabaseManager::GetFileList(string const& dir_name) {}