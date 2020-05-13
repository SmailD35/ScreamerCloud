#ifndef SCREAMER_NETWORK_SERVER_H
#define SCREAMER_NETWORK_SERVER_H

#include <string>
#include <vector>
#include "boost/asio.hpp"
#include "../../tests/headers/test_network_client.h"

enum CmdCodeClient {HELP_CLI,
		UPLOAD_CLI,
		DOWNLOAD_CLI,
		DELETE_CLI,
		LIST_CLI,
		REGISTER_CLI,
		LOGIN_CLI};

class ConnectionNetwork
{
protected:
	char * _buf_send;
	char * _buf_recv;

private:
	void Send(int buf_size);
	void Recv(int buf_size);
	int Serialize();
	map<string, string> Deserialize(int buf_size);
	boost::asio::ip::tcp::socket * socket;

public:
	ConnectionNetwork(boost::asio::ip::tcp::socket *sock);
	~ConnectionNetwork();
	void SendMsg(map<string, string> &client_query);
	map<string, string> RecvMsg();
	void SendFile(File &file_obj);
	void RecvFile(File &file_obj_ptr);
};


class ServerNetwork
{
private:
	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor;
	int _port;
	string _IP;

public:
	ServerNetwork(boost::asio::io_context io_context);
	~ServerNetwork();
	ConnectionNetwork StandConnection();
};


#endif //SCREAMER_NETWORK_SERVER_H
