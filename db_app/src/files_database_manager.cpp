//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/files_database_manager.h"

using namespace std;

FilesDatabaseManager::FilesDatabaseManager() {
   // _userID = -1;
    _userID = NIL;
    _userDirectory = "";
}

void FilesDatabaseManager::SetUserID(int userID) {
    _userID = userID;
    _databaseConnection.SetUserID(userID);
}

void FilesDatabaseManager::SetUserDirectory(string userDirectory) {
    _userDirectory = std::move(userDirectory);
}

//файл найден - true, иначе - false
bool FilesDatabaseManager::CheckExistingFile(const string &file_name, const string &dir_name) {}

////ошибки при работе с записью в БД обрабатываются исключениями
bool FilesDatabaseManager::AddFileRecord(const string &file_name, const string &dir_name, const string &hash_sum) {}
bool FilesDatabaseManager::DeleteFileRecord(const string &file_name, const string &dir_name) {}

///////////////////////////////////////////////////ПЕРЕДАВАТЬ НЕ ГОЛЫЕ УКАЗАТЕЛИ!!!!
FILE * FilesDatabaseManager::GetFilePtr(string const &file_name, string const &dir_name) throw (DBExceptions){
    ////если указатель на файл не может быть взят, throw
}

bool FilesDatabaseManager::UploadFile(const string &file_name, const string &dir_name, const string &hash_sum) {
    if (!CheckExistingFile(file_name, dir_name))
        return false;
    if (!AddFileRecord(file_name, dir_name, hash_sum))
        return false;
    return true;
}

///??? либо лучше возвращать nullptr ???
FILE* FilesDatabaseManager::DownloadFile(string const& file_name, string const& dir_name) throw (DBExceptions) {
    if (CheckExistingFile(file_name, dir_name)) {
        try {
            FILE *file = GetFilePtr(file_name, dir_name);
            return file;
        }
        catch (DBExceptions &exceptions) {
            exceptions.m_what(); //залогируем сообщение о том, что не удалось взять указатель на файл (системная ошибка)
        }
    }
    else
        throw DBExceptions("Such file doesn't exist");
    return nullptr; //?
}

bool FilesDatabaseManager::DeleteFile(const string &file_name, const string &dir_name) {
    if (!CheckExistingFile(file_name, dir_name))
        return false;
    if (!DeleteFileRecord(file_name, dir_name))
        return false;
    return true;
}

////////////////////ПРОДУМАТЬ
vector <string> FilesDatabaseManager::GetFileList(string const& dir_name) {
    ////if (!CheckExistingFile(file_name, dir_name)) {}
    ////проверка на существование такой директории (запись user_path в БД пользователя), если не найдено - бросаем исключение
    ////если директория найдена, вытаскиваем из БД соответсвтующие имена файлов (рекурсивный или нерекурсивный вывод списка файлов?)
    ////может быть, лучше возвращать список файлов в другой структуре
}