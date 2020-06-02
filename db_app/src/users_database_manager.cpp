//
// Created by ekaterina on 14.04.2020.
//

#include <cmd_codes.h>
#include "../inc/users_database_manager.h"
using namespace std;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

UsersDatabaseManager::UsersDatabaseManager() {
    _userID = NIL;
    _databaseConnection = DatabaseWrapper();
    SetPathToUsersStorage();
}

int UsersDatabaseManager::RegisterUser(const std::string &login, const std::string &password, DbErrorCodes &error) {
    //инициализируем userID_
    try {
        if (!_databaseConnection.CheckExistingLogin(login, error))
        {
            try {
                int add_result = _databaseConnection.AddUserRecord(login, password, error);
                _userID = add_result;
                if (add_result == FAIL)
                    return FAIL;
                _databaseConnection.SetUserID(add_result);

				fs::create_directory(_path_users_storage + '/' + to_string(add_result));
                return _userID;
            }
            catch (exception &exc) {
                BOOST_LOG_TRIVIAL(error) << exc.what();
                error = DB_REGISTRATION_ERROR;
                return FAIL;
            }
        }
        else
            return FAIL;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_APPEAL_ERROR;
        return FAIL;
    }
}

int UsersDatabaseManager::AuthorizeUser(const std::string &login, const std::string &password, DbErrorCodes &error) {
    try {
        int userID = _databaseConnection.CheckUserID(login, password, error);
        if (userID == FAIL)
            return FAIL;
        if (_userID == NIL)
            _userID = userID;

        _databaseConnection.SetUserID(userID);
        return _userID;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_APPEAL_ERROR;
        return FAIL;
    }
}

bool UsersDatabaseManager::DeleteUser(const std::string &login, const std::string &password, DbErrorCodes &error) {
    try {
        int userID = _databaseConnection.CheckUserID(login, password, error);
        if (userID != FAIL) {
            return DeleteUserInternal(userID, error);
        }
        else
            return false;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(error) << exc.what();
        error = DB_DELETING_USER_ERROR;
        return false;
    }
}


bool UsersDatabaseManager::DeleteUserInternal(int userID, DbErrorCodes &error) {
    try {
        _databaseConnection.DeleteUserRecord(userID, error);
        try {
            _databaseConnection.DeleteAllFiles(error);
            fs::remove_all(_path_users_storage + '/' + to_string(userID));
        }
        catch (exception &exc) {
            error = DB_DELETING_ALL_FILES_ERROR;
            BOOST_LOG_TRIVIAL(error) << exc.what();
            return false;
        }
        return true;
    }
    catch (exception &exc) {
        error = DB_DELETING_USER_ERROR;
        BOOST_LOG_TRIVIAL(error) << exc.what();
        return false;
    }
}


void UsersDatabaseManager::SetPathToUsersStorage() {
	////считываем из конфига путь к хранящимся файлам пользователей
	pt::ptree root;
	pt::read_json("/etc/screamer_cloud.conf", root);
	string users_path = root.get<string>("users_storage");

	if (!fs::exists(users_path))
		fs::create_directory(users_path);
	_path_users_storage = users_path;
}
