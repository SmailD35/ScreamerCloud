//
// Created by ekaterina on 18.04.2020.
//

#include "../inc/database_wrapper.h"
using namespace std;


DatabaseWrapper::DatabaseWrapper() {
    GetUsersDBInfo();
    GetFilesDBInfo();
}

int DatabaseWrapper::CheckUserData(string const& login, string const& password) {

}

int DatabaseWrapper::AddUserRecord(const string &login, const string &password) throw (DBExceptions) {
    //return 0;
}

int DatabaseWrapper::DeleteUserRecord(const string &login, const string &password) throw (DBExceptions) {
    //return 0;
}

int DatabaseWrapper::CheckExistingFile(const string &file_name, const string &dir_name) {

}

int DatabaseWrapper::AddFileRecord(const string &file_name, const string &dir_name, const string &hash_sum) throw (DBExceptions) {
    return 0;
}

int DatabaseWrapper::DeleteFileRecord(const string &file_name, const string &dir_name) throw (DBExceptions) {
    return 0;
}

int DatabaseWrapper::GetFileID(const string &file_name, const string &dir_name) throw (DBExceptions) {
    return 0;
}

vector<string> DatabaseWrapper::GetFileList(const string &dir_name) {
    return vector<string>();
}

shared_ptr<PGconn> DatabaseWrapper::GetConnection(DBType db_type) {
    shared_ptr<PGconn> connection;
    if (db_type == USERS_DB) {
        connection.reset(PQconnectdb(_users_db_info.c_str()));
        ////изменить функцию подключения на ту, где будет необходим пароль (или можно ли в эту стркоу подключения пихать пароль?)
    }
    else {
        connection.reset(PQconnectdb(_files_db_info.c_str()));
    }

    return connection;
}

int DatabaseWrapper::CloseConnection(DBType db_type) {
    return 0;
}

void DatabaseWrapper::GetFilesDBInfo() {
    ////строка с данными для подключения бует браться из конфиг-файла
    _files_db_info = "dbname=files_db user=screamer_cloud password=1221";
}

void DatabaseWrapper::GetUsersDBInfo() {
    _users_db_info = "dbname=users_db user=screamer_cloud password=1221";
}

void DatabaseWrapper::SetUserID(int userID) {
    _userID = userID;
}

bool DatabaseWrapper::DeleteUserRecord(int userID) throw (DBExceptions){
    return true;
}

