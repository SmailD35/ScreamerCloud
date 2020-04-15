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

int FilesDatabaseManager::CheckExistingFile() {}

int FilesDatabaseManager::AddFileRecord() {}

int FilesDatabaseManager::DeleteFileRecord() {}

FILE* FilesDatabaseManager::GetFilePtr() {}

int FilesDatabaseManager::UploadFile(string const& file_name, string const& dir_name, string const& hash_sum) {
    if (CheckExistingFile())
        return 1;
    if (AddFileRecord())
        return 1;
    return 0;
}

FILE* FilesDatabaseManager::DownloadFile(string const& file_name, string const& dir_name) {
    if (CheckExistingFile())
        return nullptr;
    return GetFilePtr();
}

int FilesDatabaseManager::DeleteFile(string const& file_name, string const& dir_name ) {
    if (CheckExistingFile())
        return 1;
    if (DeleteFileRecord())
        return 1;
    return 0;
}

vector <string> FilesDatabaseManager::GetFileList(string const& dir_name) {}