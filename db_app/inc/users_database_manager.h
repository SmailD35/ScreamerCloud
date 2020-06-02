//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_USERS_DATABASE_MANAGER_H
#define DB_APP_USERS_DATABASE_MANAGER_H

#include "database_wrapper.h"

class UsersDatabaseManager {
private:
    int _userID;

    DatabaseWrapper _databaseConnection;

	std::string _path_users_storage;

	void SetPathToUsersStorage();

    bool DeleteUserInternal(int userID, DbErrorCodes &error);
public:
    UsersDatabaseManager();

    ~UsersDatabaseManager() = default;

	int RegisterUser(const std::string &login, const std::string &password, DbErrorCodes &error);

    int AuthorizeUser(const std::string &login, const std::string &password, DbErrorCodes &error);

    bool DeleteUser(const std::string &login, const std::string &password, DbErrorCodes &error);
};


#endif //DB_APP_USERS_DATABASE_MANAGER_H
