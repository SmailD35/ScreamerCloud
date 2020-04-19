//
// Created by ekaterina on 18.04.2020.
//

#ifndef DB_APP_DATABASE_WRAPPER_H
#define DB_APP_DATABASE_WRAPPER_H

#include <string>
#include <vector>
//#include <libpq-fe.h>
#include "stubs_libpq.h"

#define CONNECT_TO_USERS_DB 0
#define CONNECT_TO_FILES_DB 1

using namespace std;

class DatabaseConnection {
private:
    char* _files_db_name;

    char* _users_db_name;

    int _userID;

    virtual PGconn GetConnection(int db_type) = 0;

    virtual int CloseConnection(int db_type) = 0;

public:
    DatabaseConnection() = default;
    ~DatabaseConnection() = default;

    //имена берутся из конфига?
    virtual void SetFilesDBName() = 0;
    virtual void SetUsersDBName() = 0;

    virtual void SetUserID(int userID) = 0;

    virtual int CheckUserData(string const& login, string const& password) = 0;

    virtual int AddUserRecord(string const& login, string const& password) = 0;

    virtual int DeleteUserRecord(string const& login, string const& password) = 0;

    //------------------------------------------------------------------------------

    virtual int CheckExistingFile(string const& file_name, string const& dir_name) = 0;

    virtual int AddFileRecord(string const& file_name, string const& dir_name, string const& hash_sum) = 0;

    virtual int DeleteFileRecord(string const& file_name, string const& dir_name) = 0;

    virtual int GetFileID(string const& file_name, string const& dir_name) = 0;

    virtual vector <string> GetFileList(string const& dir_name) = 0;
};


#endif //DB_APP_DATABASE_WRAPPER_H
