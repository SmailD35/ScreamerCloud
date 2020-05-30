#ifndef NETWORK_NETWORK_CLIENT_H
#define NETWORK_NETWORK_CLIENT_H

#include <string>
#include <vector>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "file.h"
#include <memory>
//#include "../../tests/headers/test_network_client.h"

enum ErrorNetworkCode {
	ERROR_CONNECTION = -1,
	ERROR_RECV = -2,
	ERROR_SEND = -3,
	ERROR_INPUT_MAP = -4,
	};

class ClientNetwork
{
private:
	boost::asio::io_service _io_service;
	boost::asio::ip::tcp::socket _socket;
	int _port;
	std::string _IP;

protected:
	std::string buf_send;
	std::string buf_recv;

private:
	void Serialize(std::map<std::string, std::string> &client_query);
	std::shared_ptr<std::map<std::string, std::string>> Deserialize();
	int Send();
	int Recv();

public:
	ClientNetwork(const std::string& IP, int port);
	~ClientNetwork();
	int Connection();
	int SendMsg(std::map<std::string, std::string> &client_query);
	std::shared_ptr<std::map<std::string, std::string>> RecvMsg();
	int SendFile(const std::shared_ptr<InFile>& file_obj);
	int RecvFile(const std::shared_ptr<OutFile>& file_obj);
};


class ClientNetworkTest : public ClientNetwork
{
public:
	std::string GetBuf(int choice);
	void SetBuf(int choice, std::string buf);
};



#endif //NETWORK_NETWORK_CLIENT_H
