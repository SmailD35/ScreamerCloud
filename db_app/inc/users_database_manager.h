//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_USERS_DATABASE_MANAGER_H
#define DB_APP_USERS_DATABASE_MANAGER_H

#include <string>
#include "database_connection.h"

using namespace std;

class UsersDatabaseManager {
private:
    int _userID;

    int AddUserRecord(string const &login, string const &password);

    int CheckUserData(string const &login, string const &password);

    int DeleteUserRecord(string const &login, string const &password);

    //храним указатель, поскольку класс абстрактный
    DatabaseConnection *_databaseConnection;
public:
    UsersDatabaseManager();
    ~UsersDatabaseManager() = default;

    int RegisterUser(string const& login, string const& password);

    int AuthorizeUser(string const& login, string const& password);

    int DeleteUser(string const& login, string const& password);
};


#endif //DB_APP_USERS_DATABASE_MANAGER_H
