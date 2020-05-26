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

public:
    UsersDatabaseManager();

    ~UsersDatabaseManager() = default;

	int RegisterUser(const std::string &login, const std::string &password);

    int AuthorizeUser(const std::string &login, const std::string &password);

    bool DeleteUser(const std::string &login, const std::string &password);
};


#endif //DB_APP_USERS_DATABASE_MANAGER_H
