#ifndef SCREAMER_NETWORK_SERVER_H
#define SCREAMER_NETWORK_SERVER_H

#include <string>
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <memory>
#include "file.h"
//#include "../../tests/headers/test_network_client.h"

enum ErrorNetworkCode {
	ERROR_CONNECTION = -1,
	ERROR_RECV = -2,
	ERROR_SEND = -3,
	ERROR_INPUT_MAP = -4,
};

class ConnectionNetwork
{
protected:
	std::string _buf_send;
	std::string _buf_recv;

private:
	void Serialize(const std::shared_ptr<std::map<std::string, std::string>> &server_answer);
	std::shared_ptr<std::map<std::string, std::string>> Deserialize();
	int Send();
	int Recv();
	boost::shared_ptr<boost::asio::ip::tcp::socket> socket;
	//boost::asio::ip::tcp::socket * socket;

public:
	ConnectionNetwork(boost::shared_ptr<boost::asio::ip::tcp::socket> sock);
	//ConnectionNetwork(boost::asio::ip::tcp::socket * sock);
	~ConnectionNetwork();
	void CloseConnection();
	int SendMsg(const std::shared_ptr<std::map<std::string, std::string>> &client_query);
	std::shared_ptr<std::map<std::string, std::string>> RecvMsg();
	int SendFile(const std::shared_ptr<InFile> &file_obj);
	int RecvFile(const std::shared_ptr<OutFile> &file_obj);
};


class ServerNetwork
{
private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::acceptor _acceptor;
	int _port;
	std::string _IP;

public:
	ServerNetwork(const std::string& IP, int port);
	~ServerNetwork();
	ConnectionNetwork StandConnection();
};

#endif //SCREAMER_NETWORK_SERVER_H