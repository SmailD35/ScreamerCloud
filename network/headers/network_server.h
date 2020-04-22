#ifndef SCREAMER_NETWORK_SERVER_H
#define SCREAMER_NETWORK_SERVER_H

#include <string>
#include <vector>
#include "../tests/headers/test_network_client.h"


using namespace std;


class ConnectionNetwork
{
private:
	void Send(int buf_size);
	void Recv(int buf_size);
	int Serialize();
	map<string, string> Deserialize(int buf_size);

protected:
	char * _buf_send;
	char * _buf_recv;

public:
	ConnectionNetwork();
	~ConnectionNetwork();
	void SendMsg(map<string, string> client_query);
	map<string, string> RecvMsg();
	void SendFile(File * file_obj);
	void RecvFile(File * file_obj_ptr);
};


class ServerNetwork
{
private:
	//ip::tcp::socket _socket;
	int _port;
	string _IP;

public:
	ServerNetwork();
	~ServerNetwork();
	ConnectionNetwork StandConnection();
};


#endif //SCREAMER_NETWORK_SERVER_H
