#ifndef NETWORK_NETWORK_CLIENT_H
#define NETWORK_NETWORK_CLIENT_H

#include <string>
#include <vector>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

using namespace std;
using namespace boost::asio;

class ClientNetwork
{
private:
	ip::tcp::socket _socket;
	int _port;
	std::string _IP;
	char * buf_send;
	char * buf_recv;
	void Send(int buf_size);
	void Recv(int buf_size);
	int Serialize(map<string, string>);
	map<string, string> Deserialize(int buf_size);

public:
	ClientNetwork();
	~ClientNetwork();
	void SendMsg(map<string, string> client_query);
	map<string, string> RecvMsg();
	void SendFile(File file_obj);
	void RecvFile(File * file_obj_ptr);
};



#endif //NETWORK_NETWORK_CLIENT_H
