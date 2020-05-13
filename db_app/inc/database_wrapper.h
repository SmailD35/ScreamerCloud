//
// Created by ekaterina on 18.04.2020.
//

#ifndef DB_APP_DATABASE_WRAPPER_H
#define DB_APP_DATABASE_WRAPPER_H

#include <string>
#include <vector>

#include <libpq-fe.h>
#include <iostream>

#include <memory>

enum Msg{
    NIL = -2,
    FAIL = -1
};

enum DBType{
    USERS_DB,
    FILES_DB
};

class DBExceptions {
private:
    std::string m_error;

public:
    DBExceptions(std::string error) : m_error(error) {}
    void m_what() { std::cout << m_error; } ////сделать вывод не в iostream, а в логи
};


////добавить логирование и чтение данных из конфиг-файла
class DatabaseWrapper {
private:
    std::string _files_db_info;

    std::string _users_db_info;

    int _userID;

    //std::shared_ptr<PGconn>  _connection;

    /////нужно ли каждый раз создавать подключение или можно его хранить в объекте ?
    ////по идее надо каждый раз создавать новое, чтобы подключаться то к users, то к files
    std::shared_ptr <PGconn> GetConnection(DBType db_type);

    int CloseConnection(DBType db_type);

    //имена берутся из конфига?
    void GetFilesDBInfo();
    void GetUsersDBInfo();

public:
    DatabaseWrapper();
    ~DatabaseWrapper() = default;

    void SetUserID(int userID);

    int CheckUserData(const std::string &login, const std::string &password);

    int AddUserRecord(const std::string &login, const std::string &password) throw(DBExceptions);

    int DeleteUserRecord(const std::string &login, const std::string &password) throw(DBExceptions);

    bool DeleteUserRecord(int userID) throw(DBExceptions);

    //------------------------------------------------------------------------------

    int CheckExistingFile(const std::string &file_name, const std::string &dir_name);

    int AddFileRecord(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum) throw(DBExceptions);

    int DeleteFileRecord(const std::string &file_name, const std::string &dir_name) throw(DBExceptions);

    int GetFileID(const std::string &file_name, const std::string &dir_name) throw(DBExceptions);

    std::vector <std::string> GetFileList(std::string const& dir_name);
};


#endif //DB_APP_DATABASE_WRAPPER_H
