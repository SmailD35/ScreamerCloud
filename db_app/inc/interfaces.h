//
// Created by ekaterina on 14.04.2020.
//

#ifndef DB_APP_INTERFACES_H
#define DB_APP_INTERFACES_H

#include "database_manager.h"
#include "database_wrapper.h"

#include <vector>
#include <string>
using namespace std;

class ConnectionNetwork {
public:
    ConnectionNetwork() = default;
    ~ConnectionNetwork() = default;
};


class UserSession {
private:
    ConnectionNetwork _userConnection;
    DatabaseManager _databaseManager;
    vector <string> _userQuery;
public:
    UserSession() {
        _userConnection = ConnectionNetwork();
        _databaseManager = DatabaseManager();
    }
};




#endif //DB_APP_INTERFACES_H
