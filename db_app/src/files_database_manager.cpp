//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/files_database_manager.h"

using namespace std;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

FilesDatabaseManager::FilesDatabaseManager() {
    _userID = NIL;
    _userDirectory = "";
    _databaseConnection = DatabaseWrapper();
    SetPathToUsersStorage();
    _databaseConnection.SetUsersStoragePath(_path_users_storage);
}

void FilesDatabaseManager::SetUserID(int userID) {
    _userID = userID;
    _databaseConnection.SetUserID(userID);
}

void FilesDatabaseManager::SetUserDirectory(string userDirectory) {
    _userDirectory = std::move(userDirectory);
}

shared_ptr<FILE> FilesDatabaseManager::GetFilePtr(int file_ID) {
    string path_to_file = _path_users_storage + _userDirectory + to_string(file_ID);

    shared_ptr<FILE> fp;
    FILE * tmp = fopen(path_to_file.c_str(), "r");
    if(tmp != nullptr)
        fp = shared_ptr<FILE>(tmp, fclose);
    else
    {
        BOOST_LOG_TRIVIAL(error) << "Can't get file ptr of " + to_string(file_ID);
        return nullptr;
    }


    return fp;
}

bool FilesDatabaseManager::UploadFile(const string &file_name, const string &dir_name, const string &hash_sum) {
    try
    {
        int file_ID = _databaseConnection.AddFileRecord(file_name, dir_name, hash_sum);

        //////для проверки тестов
       // boost::filesystem::ofstream( _path_users_storage + _userDirectory + to_string(file_ID));

        /////раскоментировать при соединении программы с свервером
        fs::path path_to_file(_path_users_storage + file_name);
        fs::path new_path(_path_users_storage + _userDirectory + to_string(file_ID));
        rename(path_to_file, new_path);
        return true;
    }
    catch(exception &exc)
    {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////тут продумать систему кодов ошибок (можно сделать еще один аргумент функций, в который будет записываться код ошибки)
shared_ptr<FILE> FilesDatabaseManager::DownloadFile(string const& file_name, string const& dir_name) {
    int fileID;
    try {
        fileID = _databaseConnection.CheckFileID(file_name, dir_name);
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(debug) << exc.what();
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
        BOOST_LOG_TRIVIAL(debug) << exc.what();
        return false;
    }

    try {
        _databaseConnection.DeleteFileRecord(fileID);
        fs::remove(_path_users_storage + _userDirectory + to_string(fileID));
        return true;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
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
        BOOST_LOG_TRIVIAL(debug) << exc.what();
    }
    return file_list;
}


void FilesDatabaseManager::SetPathToUsersStorage() {
    ////считываем из конфига путь к хранящимся файлам пользователей
    pt::ptree root;
    pt::read_json("../config.json", root);
    string users_path = root.get<string>("users_storage");

    if (!fs::exists(users_path))
        fs::create_directory(users_path);
    _path_users_storage = users_path;
}

bool FilesDatabaseManager::CheckExistingFile(const std::string &file_name, const std::string &dir_name) {
    return _databaseConnection.CheckFileID(file_name, dir_name) != 0;
}

std::string  FilesDatabaseManager::GetPathToUsersStorage() { return _path_users_storage; }