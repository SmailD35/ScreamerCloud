#ifndef SCREAMER_NETWORK_SERVER_H
#define SCREAMER_NETWORK_SERVER_H

#include <string>
#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace std;
using namespace boost::asio;


class ServerNetwork
{
private:
	ip::tcp::socket _socket;
	int _port;
	string _IP;

public:
	ServerNetwork();
	~ServerNetwork();
	UserSession StandConnection();
};

class ConnectionNetwork
{
private:
	ip::tcp::socket _socket;
	char * _buf_send;
	char * _buf_recv;
	void Send(int buf_size);
	void Recv(int buf_size);
	int Serialize();
	map<string, string> Deserialize(int buf_size);
public:
	ConnectionNetwork();
	~ConnectionNetwork();
	void SendMsg(map<string, string> client_query);
	map<string, string> RecvMsg();
	void SendFile(File file_obj);
	void RecvFile(File * file_obj_ptr);
};


#endif //SCREAMER_NETWORK_SERVER_H
