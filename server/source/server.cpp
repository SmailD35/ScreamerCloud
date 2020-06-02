//
// Created by egor on 06.05.2020.
//

#include "server.h"

using namespace std;

Server::Server()
{
	InitServerLogging();
	_config = ReadConfig();
	if (_config.empty())
	{
		BOOST_LOG_TRIVIAL(fatal) << "ReadConfig failed";
		exit(SERVER_READ_CONFIG_ERROR);
	}
	_network = make_shared<ServerNetwork>(_config["IP"], stoi(_config["port"]));
}

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
			shared_ptr<map<string, string>> message = connection.RecvMsg();
			if (message == nullptr)
			{
				BOOST_LOG_TRIVIAL(error) << connection.GetClientIP() << " RecvMsg failed";
				continue;
			}

			UserSession userSession(connection, *message, _config["storage_path"]);
			shared_ptr<Command> newCommand = CreateCommand(userSession);
			if (newCommand == nullptr)
			{
				BOOST_LOG_TRIVIAL(error) << connection.GetClientIP() << " CreateCommand failed";
				continue;
			}
			_queries.push(newCommand);
		}
	}
}

shared_ptr<Command> Server::CreateCommand(UserSession userSession)
{
	map<string,string> &query = userSession._userQuery;

	if (query.count("error_code") && (query["error_code"] == "0"))
		switch (stoi(query["cmd_code"]))
		{
		case UPLOAD:
			return make_shared<UploadFileCommand>(userSession);

		case DOWNLOAD:
			return make_shared<DownloadFileCommand>(userSession);

		case DELETE_FILE:
			return make_shared<DeleteFileCommand>(userSession);

		case DELETE_USER:
			return make_shared<DeleteUserCommand>(userSession);

		case LIST:
			return make_shared<SendFileListCommand>(userSession);

		case REGISTER:
			return make_shared<RegisterUserCommand>(userSession);

		case LOGIN:
			return make_shared<LoginUserCommand>(userSession);

		default:
			return nullptr;
		}
}

void Server::WorkerLoop()
{
	while (true)
	{
		if (!_queries.empty())
		{
			shared_ptr<Command> command = _queries.front();
			_queries.pop();
			Invoker newInvoker(command);
			if (newInvoker.Do() != SERVER_NOERROR)
				newInvoker.Undo();
		}
	}
}

map<string, string> Server::ReadConfig(const string& path)
{
	map<string, string> result;
	pt::ptree root;

	try
	{
		pt::read_json(path, root);
		result["storage_path"] = root.get<string>("users_storage");
		for (pt::ptree::value_type& values : root.get_child("server"))
		{
			if (values.first == "IP")
				result["IP"] = values.second.data();
			if (values.first == "TCP_port")
				result["port"] = values.second.data();
		}
		if (!(result.count("storage_path") && result.count("IP") && result.count("port")))
		{
			result.clear();
			throw runtime_error("Missing crucial data in the config");
		}
	}
	catch (std::exception &exc)
	{
		BOOST_LOG_TRIVIAL(fatal) << exc.what();
	}

	return result;
}