//
// Created by ekaterina on 18.04.2020.
//

#ifndef DB_APP_DATABASE_WRAPPER_H
#define DB_APP_DATABASE_WRAPPER_H

#include <string>
#include <vector>

extern "C"
{
    #include <libpq-fe.h>
}

#include <iostream>
#include <fstream>
#include <memory>
#include <boost/filesystem.hpp>
#include <boost/filesystem/directory.hpp>

#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

#include <regex>
#include "file.h"
#include "logging.hpp"

enum Msg{
    NIL = -2,
    FAIL = -1
};

enum DBType{
    USERS_DB,
    FILES_DB
};

////добавить логирование и чтение данных из конфиг-файла

class DatabaseWrapper {
private:
    std::string _files_db_info;

    std::string _users_db_info;

    int _userID;

    std::shared_ptr <PGconn> GetConnection(DBType db_type);

    std::string _users_storage_path;

    void GetFilesDBInfo();

    void GetUsersDBInfo();

public:
    DatabaseWrapper();

    ~DatabaseWrapper() = default;

    void SetUserID(int userID);

    int CheckUserID(const std::string &login, const std::string &password);

    int AddUserRecord(const std::string &login, const std::string &password);

    void DeleteUserRecord(int userID);

    bool CheckExistingLogin(const std::string &login);

    //------------------------------------------------------------------------------

    int CheckFileID(const std::string &file_name, const std::string &dir_name);

    int AddFileRecord(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum);

    void DeleteFileRecord(int fileID);

    void DeleteAllFiles();

    std::map <std::string, std::string> GetFileList(std::string const& dir_name);
};


#endif //DB_APP_DATABASE_WRAPPER_H
