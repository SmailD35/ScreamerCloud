//
// Created by egor on 06.05.2020.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <queue>
#include "commands.h"
#include "network_server.h"

class Server
{
private:
	ServerNetwork _network;
	queue<ConnectionNetwork> _connections;
	queue<Command*> _queries;
	Command* CreateCommand(UserSession userSession);

public:

	void ConnectionsLoop();
	void QueriesLoop();
	void WorkerLoop();
};

#endif //SERVER_SERVER_H