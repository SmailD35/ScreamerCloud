//
// Created by egor on 06.05.2020.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H

#include <queue>
#include "commands.h"
#include "network_server.h"
#include "file.h"
#include <boost/property_tree/json_parser.hpp>
#include "cmd_codes.h"
#include "error_codes.h"
#include "server_logging.hpp"

namespace pt = boost::property_tree;

class Server
{
public:
	Server();
	std::map<std::string, std::string> ReadConfig(const std::string& path = "/etc/screamer_cloud.conf");
	void ConnectionsLoop();
	void QueriesLoop();
	void WorkerLoop();

private:
	std::queue<ConnectionNetwork> _connections;
	std::queue<std::shared_ptr<Command>> _queries;
	std::shared_ptr<Command> CreateCommand(UserSession userSession);
	std::shared_ptr<ServerNetwork> _network;
	std::map<std::string, std::string> _config;
};

#endif //SERVER_SERVER_H