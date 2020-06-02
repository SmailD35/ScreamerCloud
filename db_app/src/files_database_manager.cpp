//
// Created by ekaterina on 14.04.2020.
//

#include <cmd_codes.h>
#include "../inc/files_database_manager.h"

using namespace std;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

FilesDatabaseManager::FilesDatabaseManager() {
    _userID = NIL;
    _userDirectory = "";
    _databaseConnection = DatabaseWrapper();
    SetPathToUsersStorage();
}

void FilesDatabaseManager::SetUserID(int userID) {
    _userID = userID;
    _databaseConnection.SetUserID(userID);
}

void FilesDatabaseManager::SetUserDirectory(string userDirectory) {
    _userDirectory = move(userDirectory);
}

shared_ptr<InFile> FilesDatabaseManager::GetFilePtr(int file_ID) {
	string path = _path_users_storage + '/' + _userDirectory + '/' + to_string(file_ID);
    shared_ptr<InFile> in_file = make_shared<InFile>(path);
	return in_file;
}

bool
FilesDatabaseManager::UploadFile(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum,
                                 DbErrorCodes &error) {
    try
    {
        int file_ID = _databaseConnection.AddFileRecord(file_name, dir_name, hash_sum, error);

        if (file_ID == FAIL)
            return false;
        BOOST_LOG_TRIVIAL(debug) << "file_ID " << file_ID << endl;
        //////для проверки тестов
       // boost::filesystem::ofstream( _path_users_storage + _userDirectory + to_string(file_ID));

        /////раскоментировать при соединении программы с свервером
        fs::path path_to_file(_path_users_storage + '/' + _userDirectory + '/' + file_name);
        BOOST_LOG_TRIVIAL(debug)<< "path_to_file " << path_to_file << endl;
        fs::path new_path(_path_users_storage + '/' + _userDirectory + '/' + to_string(file_ID));
        BOOST_LOG_TRIVIAL(debug) << "new path " << new_path << endl;
        rename(path_to_file, new_path);
        return true;
    }
    catch(exception &exc)
    {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_ADDING_FILE_RECORD;
        return false;
    }
}


std::shared_ptr<InFile>
FilesDatabaseManager::DownloadFile(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error) {
	int fileID;
	try {
		fileID = _databaseConnection.CheckFileID(file_name, dir_name, error);
	}
	catch (exception &exc) {
		BOOST_LOG_TRIVIAL(error) << exc.what();
		error = DB_APPEAL_ERROR;
		return nullptr;
	}

    shared_ptr<InFile> in_file = GetFilePtr(fileID);
    return in_file;
}

bool FilesDatabaseManager::DeleteFile(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error) {
    int fileID;
    try {
         fileID = _databaseConnection.CheckFileID(file_name, dir_name, error);
         if (fileID == FAIL)
             return false;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_APPEAL_ERROR;
        return false;
    }

    try {
        _databaseConnection.DeleteFileRecord(fileID, error);
        fs::remove(_path_users_storage + '/' + _userDirectory + '/' + to_string(fileID));
        return true;
    }
    catch (exception &exc) {
        error = DB_DELETING_FILE_ERROR;
        BOOST_LOG_TRIVIAL(error) << exc.what();
        return false;
    }
}



///нерекурсивный вывод файлов соответствующей директории
std::map<std::string, std::string> FilesDatabaseManager::GetFileList(const std::string &dir_name, DbErrorCodes &error) {
    map<string, string> file_list;
    try {
        file_list = _databaseConnection.GetFileList(dir_name, error);
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_APPEAL_ERROR;
    }
    return file_list;
}


void FilesDatabaseManager::SetPathToUsersStorage() {
    ////считываем из конфига путь к хранящимся файлам пользователей
    pt::ptree root;
    pt::read_json("/etc/screamer_cloud.conf", root);
    string users_path = root.get<string>("users_storage");

    if (!fs::exists(users_path))
        fs::create_directory(users_path);
    _path_users_storage = users_path;
}

bool FilesDatabaseManager::CheckExistingFile(const std::string &file_name, const std::string &dir_name,
                                             DbErrorCodes &error) {
    return _databaseConnection.CheckFileID(file_name, dir_name, error) != 0;
}

string  FilesDatabaseManager::GetPathToUsersStorage() { return _path_users_storage; }

std::string
FilesDatabaseManager::GetPublicLink(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error) {
    string file_link;
    int fileID;
    try {
        fileID = _databaseConnection.CheckFileID(file_name, dir_name, error);
        if (fileID == FAIL)
            return file_link;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_APPEAL_ERROR;
        return file_link;
    }

    try {
        file_link = _databaseConnection.GetPublicLink(fileID, error);
        return file_link;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_APPEAL_ERROR;
        return file_link;
    }
}


std::shared_ptr<InFile> FilesDatabaseManager::DownloadFileByLink(const std::string &link, DbErrorCodes &error)
{
    int fileID;
    try {
        fileID = _databaseConnection.CheckFileIDByLink(link, error);
    }
    catch (exception &exc) {
        error = DB_APPEAL_ERROR;
        BOOST_LOG_TRIVIAL(error) << exc.what();
        return nullptr;
    }

    shared_ptr<InFile> in_file = GetFilePtr(fileID);
    return in_file;
}