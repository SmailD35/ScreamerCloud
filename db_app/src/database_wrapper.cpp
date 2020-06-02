//
// Created by ekaterina on 18.04.2020.
//

#include <cmd_codes.h>
#include "../inc/database_wrapper.h"
using namespace std;
using boost::lexical_cast;
namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

DatabaseWrapper::DatabaseWrapper() {
    GetUsersDBInfo();
    GetFilesDBInfo();
}

int DatabaseWrapper::CheckUserID(const std::string &login, const std::string &password, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try
    {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "SELECT ID_user FROM users_data WHERE login = '" + login + "' AND password = '" + password + "'";
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));

    //если не нашли нужной записи, возвращаем FAIL
    if (!PQntuples(query_result.get()))
        return FAIL;
    else {
        string result;
        result = PQgetvalue(query_result.get(), 0, 0);
        return lexical_cast<int>(result);
    }
}


bool DatabaseWrapper::CheckExistingLogin(const std::string &login, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try
    {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "SELECT ID_user FROM users_data WHERE login = '" + login + "'";
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));

    return PQntuples(query_result.get());
}

int DatabaseWrapper::AddUserRecord(const std::string &login, const std::string &password, DbErrorCodes &error) {
    int user_ID = FAIL;
    shared_ptr<PGconn> connection;
    try
    {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc)
    {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    user_ID = std::hash<std::string>{}(login);
    std::string hash = std::to_string(user_ID);

    std::string query = "INSERT INTO users_data VALUES (" + hash + ", '"+ login + "', '" + password + "')";
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return lexical_cast<int>(hash);
}


int DatabaseWrapper::CheckFileID(const std::string &file_name, const std::string &dir_name, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "SELECT ID_file FROM files_data WHERE ID_user = " + std::to_string(_userID) +  " AND user_path = '" + dir_name + "' AND file_name = '" + file_name + "'";
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));

    //если не нашли нужной записи, возвращаем FAIL
    if (!PQntuples(query_result.get()))
        return FAIL;
    else {
        string result;
        result = PQgetvalue(query_result.get(), 0, 0);
        return lexical_cast<int>(result);
    }
}

int DatabaseWrapper::AddFileRecord(const std::string &file_name, const std::string &dir_name, const std::string &hash_sum,
                               DbErrorCodes &error) {
    int file_ID = FAIL;
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    file_ID = std::hash<std::string>{}(to_string(_userID) + file_name + dir_name);
    std::string hash = std::to_string(file_ID);

    std::string query = "INSERT INTO files_data VALUES (" + std::to_string(_userID) + ", '" + dir_name + "' , '" + file_name + "', " + hash_sum + ", " + hash + ")";
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return lexical_cast<int>(hash);
}

std::map<std::string, std::string> DatabaseWrapper::GetFileList(std::string const &dir_name, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "SELECT user_path, file_name FROM files_data WHERE ID_user = " + std::to_string(_userID) +  " AND user_path LIKE '" + dir_name + "%'";
    BOOST_LOG_TRIVIAL(trace) << query;

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
        	fs::path pa(dir);
            file_list[pa.filename().string()] = "dir";
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
        throw std::runtime_error(PQerrorMessage(connection.get()));
    }

    return connection;
}

void DatabaseWrapper::GetFilesDBInfo() {
    pt::ptree root;
    pt::read_json("/etc/screamer_cloud_config.json", root);

    string  info;

    for (pt::ptree::value_type &values : root.get_child("files_db_info"))
    {
        info += values.first + "=";
        info += values.second.data() + " ";
    }
    _files_db_info = move(info);
}

void DatabaseWrapper::GetUsersDBInfo() {
    pt::ptree root;
    pt::read_json("/etc/screamer_cloud_config.json", root);

    string info;

    for (pt::ptree::value_type &values : root.get_child("users_db_info"))
    {
        info += values.first + "=";
        info += values.second.data() + " ";
    }
   _users_db_info = move(info);
}

void DatabaseWrapper::SetUserID(int userID) {
    _userID = userID;
}

void DatabaseWrapper::DeleteUserRecord(int userID, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(USERS_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "DELETE FROM users_data WHERE id_user = " + std::to_string(userID);
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return;
}

void DatabaseWrapper::DeleteFileRecord(int fileID, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "DELETE FROM files_data WHERE id_file = " + std::to_string(fileID);
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return;
}

void DatabaseWrapper::DeleteAllFiles(DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "DELETE FROM files_data WHERE id_user = " + to_string(_userID);
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return;
}

std::string DatabaseWrapper::GetPublicLink(int fileID, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    int link = hash<string>{}(to_string(fileID));
    string file_link = "screamer_cloud/link/file/" + to_string(link);

    std::string query = "UPDATE files_data SET public_link = " + file_link + " WHERE id_file = " + to_string(fileID);
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_COMMAND_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));
    else
        return file_link;
}

int DatabaseWrapper::CheckFileIDByLink(const std::string &link, DbErrorCodes &error) {
    shared_ptr<PGconn> connection;
    try {
        connection = GetConnection(FILES_DB);
    }
    catch(std::exception &exc) {
        error = DB_CONNECTION_ERROR;
        BOOST_LOG_TRIVIAL(fatal) << exc.what();
    }

    std::string query = "SELECT ID_file FROM files_data WHERE public_link = " + link;
    BOOST_LOG_TRIVIAL(trace) << query;

    auto res_deleter = [](PGresult* r) { PQclear(r);};
    std::unique_ptr <PGresult, decltype(res_deleter)> query_result(PQexec(connection.get(), query.c_str()), res_deleter);

    if (PQresultStatus(query_result.get()) != PGRES_TUPLES_OK)
        throw std::runtime_error(PQresultErrorMessage(query_result.get()));

    //если не нашли нужной записи, возвращаем FAIL
    if (!PQntuples(query_result.get()))
        return FAIL;
    else {
        string result;
        result = PQgetvalue(query_result.get(), 0, 0);
        return lexical_cast<int>(result);
    }
}
