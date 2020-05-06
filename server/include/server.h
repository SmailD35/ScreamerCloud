//
// Created by egor on 06.05.2020.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <queue>
#include "commands.h"
#include "network_mock.h"
#include "database_mock.h"

class Server
{
private:
	//указатель потому что тесты
	ServerNetwork* _network;
	queue<ConnectionNetwork> _connections;
	queue<Command> _queries;
	Command* CreateCommand(UserSession userSession);

public:

	void ConnectionsLoop();
	void QueriesLoop();
	void ExecuteCommand();
};


#endif //SERVER_SERVER_H
