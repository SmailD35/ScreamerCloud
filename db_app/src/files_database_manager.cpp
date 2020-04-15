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

int FilesDatabaseManager::UploadFile(string const& file_name, string const& dir_name, string const& hash_sum) {}

FILE* FilesDatabaseManager::DownloadFile(string const& file_name, string const& dir_name) {}

int FilesDatabaseManager::DeleteFile(string const& file_name, string const& dir_name ) {}

vector <string> FilesDatabaseManager::GetFileList(string const& dir_name) {}