//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/users_database_manager.h"
using namespace std;
namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

UsersDatabaseManager::UsersDatabaseManager() {
    _userID = NIL;
    _databaseConnection = DatabaseWrapper();
    SetPathToUsersStorage();
}

int UsersDatabaseManager::RegisterUser(string const& login, string const& password) {
    //инициализируем userID_
    try {
        if (!_databaseConnection.CheckExistingLogin(login))
        {
            try {
                int add_result = _databaseConnection.AddUserRecord(login, password);
                _userID = add_result;
                _databaseConnection.SetUserID(add_result);

				fs::create_directory(_path_users_storage + '/' + to_string(add_result));
                return _userID;
            }
            catch (exception &exc) {
                BOOST_LOG_TRIVIAL(error) << exc.what();
                return FAIL;
            }
        }
        else
            return FAIL;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(debug) << exc.what();
        return FAIL;
    }
}

int UsersDatabaseManager::AuthorizeUser(string const& login, string const& password) {
    try {
        int userID = _databaseConnection.CheckUserID(login, password);
        if (userID == FAIL)
            return FAIL;
        if (_userID == NIL)
            _userID = userID;

        _databaseConnection.SetUserID(userID);
        return _userID;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(debug) << exc.what();
        return FAIL;
    }
}

bool UsersDatabaseManager::DeleteUser(const string &login, const string &password) {
    try {
        int userID = _databaseConnection.CheckUserID(login, password);
        if (userID != FAIL) {
            try {
                _databaseConnection.DeleteUserRecord(userID);
                try {
                    _databaseConnection.DeleteAllFiles();
                    fs::remove_all(_path_users_storage + '/' + to_string(userID));
                }
                catch (exception &exc) {
                    BOOST_LOG_TRIVIAL(error) << exc.what();
                    return false;
                }
                return true;
            }
            catch (exception &exc) {
                BOOST_LOG_TRIVIAL(error) << exc.what();
                return false;
            }
        }
        else
            return false;
    }
    catch (exception &exc) {
        BOOST_LOG_TRIVIAL(debug) << exc.what();
        return false;
    }
}

void UsersDatabaseManager::SetPathToUsersStorage() {
	////считываем из конфига путь к хранящимся файлам пользователей
	pt::ptree root;
	pt::read_json("../config.json", root);
	string users_path = root.get<string>("users_storage");

	if (!fs::exists(users_path))
		fs::create_directory(users_path);
	_path_users_storage = users_path;
}