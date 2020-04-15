//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_USERS_DATABASE_MANAGER_H
#define DB_APP_USERS_DATABASE_MANAGER_H

#include <string>

using namespace std;

class UsersDatabaseManager{
private:
    int _userID;

    int AddUser() {}

    int CheckUserData() {}

    int DeleteUserRecord() {}
public:
    UsersDatabaseManager() {
        _userID = -1;
    }
    ~UsersDatabaseManager() = default;

    int RegisterUser(string const& login, string const& password) {
        //инициализируем userID_
        _userID = AddUser();
        return _userID;
    }

    int AuthorizeUser(string const& login, string const& password) {
        int userID = CheckUserData();
        if (userID == -1)
            return userID;

        if (_userID == -1)
            _userID = userID;

        return _userID;
    }

    int DeleteUser(string const& login, string const& password) {
        if (CheckUserData()) {
            if (DeleteUserRecord())
                return 1;
            return 0;
        }
        return 1;
    }
};


#endif //DB_APP_USERS_DATABASE_MANAGER_H
