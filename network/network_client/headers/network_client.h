#ifndef NETWORK_NETWORK_CLIENT_H
#define NETWORK_NETWORK_CLIENT_H

#include <string>
#include <vector>
#include "../../tests/headers/test_network_client.h"

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
	//ip::tcp::socket _socket;
	int _port;
	std::string _IP;

protected:
	char * buf_send;
	char * buf_recv;

private:
	void Send(int buf_size);
	void Recv(int buf_size);
	int Serialize(map<string, string> &client_query);
	map<string, string> Deserialize(int buf_size);

public:
	ClientNetwork();
	~ClientNetwork();
	void SendMsg(map<string, string> &client_query);
	map<string, string> RecvMsg();
	void SendFile(File &file_obj);
	void RecvFile(File &file_obj_ptr);
};


class ClientNetworkTest : public ClientNetwork
{
public:
	char * GetBuf(int choice);
	void SetBuf(int choice, char * buf);
};



#endif //NETWORK_NETWORK_CLIENT_H
