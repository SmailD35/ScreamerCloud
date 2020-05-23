#ifndef NETWORK_NETWORK_CLIENT_H
#define NETWORK_NETWORK_CLIENT_H

#include <string>
#include <vector>
#include <iostream>
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


class ClientNetwork
{
private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::socket * _socket;
	int _port;
	std::string _IP;

protected:
	std::string buf_send;
	std::string buf_recv;

private:
	int Serialize(std::map<std::string, std::string> &client_query);
	std::map<std::string, std::string> * Deserialize(int buf_size);
	size_t Send(int buf_size);
	size_t Recv();

public:
	ClientNetwork(const std::string& IP, int port);
	~ClientNetwork();
	void SendMsg(std::map<std::string, std::string> &client_query);
	std::map<std::string, std::string> * RecvMsg();
	int SendFile(InFile &file_obj);
	int RecvFile(OutFile &file_obj);
};


class ClientNetworkTest : public ClientNetwork
{
public:
	char * GetBuf(int choice);
	void SetBuf(int choice, char * buf);
};



#endif //NETWORK_NETWORK_CLIENT_H
