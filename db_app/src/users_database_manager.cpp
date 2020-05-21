//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/users_database_manager.h"
using namespace std;

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
                return _userID;
            }
            catch (exception &exc) {
                std::cout << exc.what() << std::endl; ///log
                return FAIL;
            }
        }
        else
            return FAIL;
    }
    catch (exception &exc) {
        std::cout << exc.what() << std::endl; ///log
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
        std::cout << exc.what() << std::endl; ///log
        return FAIL;
    }
}

bool UsersDatabaseManager::DeleteUser(const string &login, const string &password) {
    try {
        int userID = _databaseConnection.CheckUserID(login, password);
        if (userID != FAIL) {
            try {
                _databaseConnection.DeleteUserRecord(userID);
                    return true;
            }
            catch (exception &exc) {
                std::cout << exc.what() << std::endl; ///log
                return false;
            }
        }
        else
            return false;
    }
    catch (exception &exc) {
        std::cout << exc.what() << std::endl; ///log
        return false;
    }
}