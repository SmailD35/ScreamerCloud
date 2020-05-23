#include "network_client.h"

namespace io = boost::asio;
using namespace std;

ClientNetwork::ClientNetwork(const string& IP, int port)
{
	_IP = IP;
	_port = port;
	_socket = new io::ip::tcp::socket(_io_service);
	io::ip::tcp::endpoint ep(io::ip::address::from_string(_IP), _port);
	_socket->connect(ep);
};

std::size_t ClientNetwork::Send(int buf_size)
{
	return boost::asio::write(*_socket, boost::asio::buffer(buf_send));
};

void ClientNetwork::Recv()
{
	boost::asio::streambuf buf;
	std::size_t read_size = boost::asio::read( *_socket, buf);
	buf_recv = boost::asio::buffer_cast<const char*>(buf.data());
};

int ClientNetwork::Serialize(map<string, string> &client_query)
{
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << client_query;
	buf_send = ss.str();
	cout << buf_send << endl;
};

map<string, string> * ClientNetwork::Deserialize(int buf_size)
{
	auto * new_map = new std::map<string , string>;
	std::stringstream ss;
	ss << buf_recv;
	boost::archive::text_iarchive iarch(ss);
	iarch >> *new_map;
	return new_map;
};

void ClientNetwork::SendMsg(map<string, string> &client_query)
{
	int msg_size = Serialize(client_query);
	if(msg_size <= 0)
		return;
	Send(msg_size);
};

map<string, string> * ClientNetwork::RecvMsg()
{
	Recv();
	map<string, string> * server_answer = Deserialize(0);
	return server_answer;
}

ClientNetwork::~ClientNetwork()
= default;

void ClientNetwork::SendFile(File &file_obj)
{
	int file_size = file_obj.GetSize();
	int send_bytes = 0;
	//int loop_count = file_size / 1024 + 1;

	for (int i = 0; send_bytes < file_size; ++i)
	{

		send_bytes += Send(1024);
	}
};

void ClientNetwork::RecvFile(File &file_obj_ptr)
{

};

char * ClientNetworkTest::GetBuf(int choice) {};

void ClientNetworkTest::SetBuf(int choice, char *buf) {};