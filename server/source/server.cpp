//
// Created by egor on 06.05.2020.
//

#include "server.h"
#include "cmd_codes.h"

using namespace std;

Server::Server(string ip, int port)
{
	_network = new ServerNetwork(ip, port);
}
;
void Server::ConnectionsLoop()
{
	while (true)
	{
		ConnectionNetwork newConnection = _network->StandConnection();
		_connections.push(newConnection);
	}
}

void Server::QueriesLoop()
{
	while (true)
	{
		if (!_connections.empty())
		{
			ConnectionNetwork connection = _connections.front();
			_connections.pop();

			map<string,string>* message = connection.RecvMsg();
			UserSession userSession(connection);
			userSession._userQuery = move(*message);
			Command* newCommand = CreateCommand(userSession);
			_queries.push(newCommand);
		}
	}
}

Command* Server::CreateCommand(UserSession userSession)
{
	map<string,string> query = userSession._userQuery;
	Command* newCommand = nullptr;

	if (query.count("error_code") && (query["error_code"] == "0"))
		switch (stoi(query["cmd_code"]))
		{
		case UPLOAD:
			newCommand = new SendFileCommand(userSession);
			break;

		case DOWNLOAD:
			newCommand = new RecvFileCommand(userSession);
			break;

		case DELETE:
			newCommand = new DeleteCommand(userSession);
			break;

		case LIST:
			newCommand = new SendFileListCommand(userSession);
			break;

		case REGISTER:
			newCommand = new RegisterUserCommand(userSession);
			break;

		case LOGIN:
			newCommand = new LoginUserCommand(userSession);
			break;
		}

	return newCommand;
}

void Server::WorkerLoop()
{
	while (true)
	{
		if (!_queries.empty())
		{
			Command* command = _queries.front();
			_queries.pop();
			Invoker newInvoker(command);
			newInvoker.Do();
		}
	}
}