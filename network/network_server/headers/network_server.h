#ifndef SCREAMER_NETWORK_SERVER_H
#define SCREAMER_NETWORK_SERVER_H

#include <string>
#include <iostream>
#include <vector>
#include "boost/asio.hpp"
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "file.h"
//#include "../../tests/headers/test_network_client.h"

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
	std::string _buf_send;
	std::string _buf_recv;

private:
//	void Send(int buf_size);
//	std::size_t Recv();
	int Serialize(std::map<std::string, std::string> &server_answer);
	std::map<std::string, std::string> * Deserialize(int buf_size);
	//boost::shared_pointer<boost::asio::ip::tcp::socket>
	boost::asio::ip::tcp::socket * socket;

public:
	void Send(int buf_size);
	std::size_t Recv();
	ConnectionNetwork(boost::asio::ip::tcp::socket *sock);
	~ConnectionNetwork();
	void SendMsg(std::map<std::string, std::string> &client_query);
	std::map<std::string, std::string> * RecvMsg();
	void SendFile(File &file_obj);
	void RecvFile(File &file_obj_ptr);
};


class ServerNetwork
{
private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::acceptor * _acceptor;
	int _port;
	std::string _IP;

public:
	ServerNetwork(const std::string& IP, int port);
	~ServerNetwork();
	ConnectionNetwork StandConnection();
};

#endif //SCREAMER_NETWORK_SERVER_H