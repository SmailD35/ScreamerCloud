//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_INTERFACES_H
#define DB_APP_INTERFACES_H

#include "database_manager.h"
#include <vector>

class ConnectionNetwork {
public:
    ConnectionNetwork() = default;
    ~ConnectionNetwork() = default;
};


class UserSession {
private:
    ConnectionNetwork _userConnection;
   // int _userID;
    DatabaseManager _databaseManager;
    std::vector <std::string> _userQuery;
public:
    UserSession() {
        _userConnection = ConnectionNetwork();
        _databaseManager = DatabaseManager();
    }

};


#endif //DB_APP_INTERFACES_H
