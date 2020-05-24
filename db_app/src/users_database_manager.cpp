//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/users_database_manager.h"
using namespace std;
namespace fs = boost::filesystem;

UsersDatabaseManager::UsersDatabaseManager() {
    _userID = NIL;
    _databaseConnection = DatabaseWrapper();
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

                fs::create_directory(_databaseConnection.GetUsersStoragePath() + to_string(add_result));
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
                    fs::remove(_databaseConnection.GetUsersStoragePath() + to_string(userID));
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