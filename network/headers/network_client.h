#ifndef NETWORK_NETWORK_CLIENT_H
#define NETWORK_NETWORK_CLIENT_H

#include <string>
#include <vector>
#include "../tests/headers/test_network_client.h"

using namespace std;

class ClientNetwork
{
private:
	//ip::tcp::socket _socket;
	int _port;
	std::string _IP;
	void Send(int buf_size);
	void Recv(int buf_size);
	int Serialize(map<string, string>);
	map<string, string> Deserialize(int buf_size);

protected:
	char * buf_send;
	char * buf_recv;

public:
	ClientNetwork();
	~ClientNetwork();
	void SendMsg(map<string, string> client_query);
	map<string, string> RecvMsg();
	void SendFile(File * file_obj);
	void RecvFile(File * file_obj_ptr);
};


class ClientNetworkTest : public ClientNetwork
{
public:
	char * GetBuf(int choice);
	void SetBuf(int choice, char * buf);
};



#endif //NETWORK_NETWORK_CLIENT_H
