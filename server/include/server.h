//
// Created by egor on 06.05.2020.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <queue>
#include "commands.h"
#include "network_server.h"
#include "file.h"

class Server
{
public:
	Server(std::string ip, int port);
	void ConnectionsLoop();
	void QueriesLoop();
	void WorkerLoop();

private:
	std::queue<ConnectionNetwork> _connections;
	std::queue<Command*> _queries;
	Command* CreateCommand(UserSession userSession);

private:
	ServerNetwork* _network;
};

#endif //SERVER_SERVER_H