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
    unique_ptr<PGresult> query_result;
    shared_ptr<PGconn> connection = GetConnection(USERS_DB);

    std::string query = "SELECT ID_user FROM data_table WHERE login = " + login + " AND password = " + password;

    query_result.reset(PQexec(connection.get(), query.c_str()));

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));

    if (!PQntuples(query_result.get()))
        return FAIL;
    else {
        string result;
        ///проверить, что возвращает функция, если такого поля нет (пустую строку или надо использовать PQisnull)
        result = PQgetvalue(query_result.get(), 0, 0);
            //// использовать lexical cast для перевода в int
            /////и возвращаем значение
    }
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

void DatabaseWrapper::GetPathToUsersFiles() {
    ////считываем из конфига путь к хранящимся файлам пользователей
}

