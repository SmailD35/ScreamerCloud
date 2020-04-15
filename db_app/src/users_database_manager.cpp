//
// Created by ekaterina on 14.04.2020.
//

#include "../inc/users_database_manager.h"

int UsersDatabaseManager::AddUser() {}

int UsersDatabaseManager::CheckUserData() {}

int UsersDatabaseManager::DeleteUserRecord() {}

UsersDatabaseManager::UsersDatabaseManager() {
    _userID = -1;
}

int UsersDatabaseManager::RegisterUser(string const& login, string const& password) {
    //инициализируем userID_
    _userID = AddUser();
    return _userID;
}

int UsersDatabaseManager::AuthorizeUser(string const& login, string const& password) {
    int userID = CheckUserData();
    if (userID == -1)
        return userID;

    if (_userID == -1)
        _userID = userID;

    return _userID;
}

int UsersDatabaseManager::DeleteUser(string const& login, string const& password) {
    if (CheckUserData()) {
        if (DeleteUserRecord())
            return 1;
        return 0;
    }
    return 1;
}