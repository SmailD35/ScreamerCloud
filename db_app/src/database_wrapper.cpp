//
// Created by ekaterina on 18.04.2020.
//

#include "../inc/database_wrapper.h"
using namespace std;
using boost::lexical_cast;
//namespace fs = boost::filesystem;

DatabaseWrapper::DatabaseWrapper() {
    GetUsersDBInfo();
    GetFilesDBInfo();
}

int DatabaseWrapper::CheckUserID(string const& login, string const& password) {
    shared_ptr<PGconn> connection;
    try
    {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    std::string query = "SELECT ID_user FROM users_data WHERE login = '" + login + "' AND password = '" + password + "'";
    std::cout << "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get())); ////логи

    //если не нашли нужной записи, возвращаем FAIL
    if (!PQntuples(query_result.get()))
        return FAIL;
    else {
        string result;
        result = PQgetvalue(query_result.get(), 0, 0);
        cout <<  lexical_cast<int>(result) << endl;
        return lexical_cast<int>(result);
    }
}


bool DatabaseWrapper::CheckExistingLogin(const std::string &login) {
    shared_ptr<PGconn> connection;
    try
    {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    std::string query = "SELECT ID_user FROM users_data WHERE login = '" + login + "'";
    std::cout <<  "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get())); ////логи

    return PQntuples(query_result.get());
}

int DatabaseWrapper::AddUserRecord(const string &login, const string &password){
    shared_ptr<PGconn> connection;
    try
    {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc)
    {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    int user_ID = std::hash<std::string>{}(login);
    std::string hash = std::to_string(user_ID);

    std::string query = "INSERT INTO users_data VALUES (" + hash + ", '"+ login + "', '" + password + "')";
    std::cout <<  "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return lexical_cast<int>(hash);
}


int DatabaseWrapper::CheckFileID(const string &file_name, const string &dir_name) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    std::string query = "SELECT ID_file FROM files_data WHERE ID_user = " + std::to_string(_userID) +  " AND user_path = '" + dir_name + "' AND file_name = '" + file_name + "'";
    std::cout <<  "      " <<  query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get())); ////логи

    //если не нашли нужной записи, возвращаем FAIL
    if (!PQntuples(query_result.get()))
        return FAIL;
    else {
        string result;
        result = PQgetvalue(query_result.get(), 0, 0);
        cout << lexical_cast<int>(result) << endl;
        return lexical_cast<int>(result);
    }
}

int DatabaseWrapper::AddFileRecord(const string &file_name, const string &dir_name, const string &hash_sum) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    int file_ID = std::hash<std::string>{}(file_name + dir_name);
    std::string hash = std::to_string(file_ID);

    std::string query = "INSERT INTO files_data VALUES (" + std::to_string(_userID) + ", '" + dir_name + "' , '" + file_name + "', " + hash_sum + ", " + hash + ")";
    std::cout <<  "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return lexical_cast<int>(hash);
}

map<string, string> DatabaseWrapper::GetFileList(const string &dir_name) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }
    ////////////////////как мы на уровне клиента обрабатываем создание новой папки? Если она пустая, то как мы это обрабатываем на сервере?

    /////////надо сделать проверку на существование у пользователя такой директории
    ////////или же просто возвращаем пустую мапу, а сервер перед отправкой ответа пользователю проверяет ее на пустоту
    ////////если мапа пустая, то такой директории нет
    std::string query = "SELECT user_path, file_name FROM files_data WHERE ID_user = " + std::to_string(_userID) +  " AND user_path LIKE '" + dir_name + "%'";
    std::cout << "      " <<  query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));

    map<string, string> file_list;

    //если не нашли нужной записи, возвращаем пустой map
    string dir,file;
    regex reg(dir_name + "/*/*");

    for (int i = 0; i < PQntuples(query_result.get()); i++)
    {
        dir = PQgetvalue(query_result.get(), i, 0);
        file = PQgetvalue(query_result.get(), i, 1);

        if (regex_search(dir, reg) && dir != dir_name)
        {
            file_list[dir] = "dir";
            ///////////вырезать название самое поддиректории (без найденной изначально директории и поддиректорий этой поддиректории)
        }
        else
            file_list[file] = "file";
    }
    return file_list;
}

shared_ptr<PGconn> DatabaseWrapper::GetConnection(DBType db_type) {
    shared_ptr<PGconn> connection;

    if (db_type == USERS_DB)
        connection.reset(PQconnectdb(_users_db_info.c_str()), &PQfinish);
    else
        connection.reset(PQconnectdb(_files_db_info.c_str()), &PQfinish);

    if (PQstatus(connection.get()) != CONNECTION_OK)
    {
        /////запись в логи
        throw std::runtime_error(PQerrorMessage(connection.get()));
    }

    return connection;
}

void DatabaseWrapper::GetFilesDBInfo() {
    ////строка с данными для подключения бует браться из конфиг-файла
    _files_db_info = "host=localhost dbname=files_db user=screamer_cloud password=1221";
}

void DatabaseWrapper::GetUsersDBInfo() {
    _users_db_info = "host=localhost dbname=users_db user=screamer_cloud password=1221";
}

void DatabaseWrapper::SetUserID(int userID) {
    _userID = userID;
}

void DatabaseWrapper::DeleteUserRecord(int userID){
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    std::string query = "DELETE FROM users_data WHERE id_user = " + std::to_string(userID);
    std::cout <<  "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return;
}

void DatabaseWrapper::DeleteFileRecord(int fileID) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    std::string query = "DELETE FROM files_data WHERE id_file = " + std::to_string(fileID);
    std::cout <<  "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return;
}

void DatabaseWrapper::DeleteAllFiles() {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        std::cout << exc.what() << std::endl; //////вывести в лог
    }

    std::string query = "DELETE FROM files_data WHERE id_user = " + to_string(_userID);
    std::cout <<  "      " << query << std::endl;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return;
}

void DatabaseWrapper::SetUsersStoragePath(const string &path) {
    _users_storage_path = path;
}

std::string DatabaseWrapper::GetUsersStoragePath() {
    return _users_storage_path;
}

