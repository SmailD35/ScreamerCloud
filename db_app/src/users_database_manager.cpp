//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/users_database_manager.h"
using namespace std;

int UsersDatabaseManager::AddUserRecord(string const &login, string const &password) {
    try {
        return _databaseConnection.AddUserRecord(login, password);
    }
    catch (DBExceptions &exceptions) {
        exceptions.m_what();
        return FAIL;
    }
}

int UsersDatabaseManager::CheckUserData(const string &login, const string &password) {
    int userID = _databaseConnection.CheckUserData(login, password);
    if (userID == FAIL) {
        return FAIL;
    }
    else
        return userID;
}

bool UsersDatabaseManager::DeleteUserRecord(const string &login, const string &password) {
    try {
        _databaseConnection.DeleteUserRecord(login, password);
        return true;
    }
    catch (DBExceptions &exceptions) {
        exceptions.m_what();
        return false;
    }
}

bool UsersDatabaseManager::DeleteUserRecord(int userID) {
    try {
        _databaseConnection.DeleteUserRecord(userID);
        return true;
    }
    catch (DBExceptions &exceptions) {
        exceptions.m_what();
        return false;
    }
}

//как инициализировать _databaseConnection?
UsersDatabaseManager::UsersDatabaseManager() {  _userID = NIL; }

int UsersDatabaseManager::RegisterUser(string const& login, string const& password) {
    //инициализируем userID_
    int add_result = AddUserRecord(login, password);
    if (add_result == FAIL) {
        return FAIL;
    }

    _userID = add_result;
    _databaseConnection.SetUserID(add_result);
    return _userID;
}

int UsersDatabaseManager::AuthorizeUser(string const& login, string const& password) {
    int userID = CheckUserData(login, password);

    if (userID == FAIL)
        return FAIL;

    if (_userID == NIL)
        _userID = userID;

    _databaseConnection.SetUserID(userID);
    return _userID;
}

bool UsersDatabaseManager::DeleteUser(const string &login, const string &password) {
    int userData = CheckUserData(login, password);
    if (userData != FAIL) {
        if (!DeleteUserRecord(userData))
            return true;
    }
    return false;
}