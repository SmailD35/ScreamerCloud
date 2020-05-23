#include "network_server.h"

namespace io = boost::asio;
//using namespace boost::asio;
//using ip::tcp;
//using std::string;
//using std::cout;
//using std::endl;
//using std::map;
using namespace std;

string value_read(int size, int start_position, string buf)
{
	string value;
	for (int i = start_position, j = 0; i < size; ++i, ++j)
	{
		value[j] = buf[i];
	}
	return value;
}


ServerNetwork::ServerNetwork(const string& IP, int port)
{
	_IP = IP;
	_port = port;
	//boost::asio::io_service io_service;
	//boost::asio::io_service * io_service = new boost::asio::io_service
	io::ip::tcp::endpoint ep(io::ip::address::from_string(_IP), _port);
	_acceptor = new io::ip::tcp::acceptor(_io_service, ep);
	//_io_service = &io_service;
	//_acceptor = acceptor;

};

ConnectionNetwork ServerNetwork::StandConnection()
{
	auto sock = new io::ip::tcp::socket(_io_service);
	_acceptor->accept(*sock);
	ConnectionNetwork client_connect(sock);
	return client_connect;
}

ServerNetwork::~ServerNetwork()
= default;

ConnectionNetwork::ConnectionNetwork(io::ip::tcp::socket *sock)
{
	socket = std::move(sock);
};

void ConnectionNetwork::Send(int buf_size)
{
	boost::asio::write(*socket, boost::asio::buffer(_buf_send));
};

std::size_t ConnectionNetwork::Recv()
{
	boost::asio::streambuf buf;
	std::size_t read_size;
	try
	{
		read_size = boost::asio::read(*socket, buf);
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
	}
	_buf_recv = boost::asio::buffer_cast<const char*>(buf.data());
	cout << _buf_recv << endl;
	return read_size;
};

int ConnectionNetwork::Serialize(map<string, string> &server_answer)
{
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << server_answer;
	_buf_send = ss.str();
	cout << _buf_send << endl;
};

map<string, string> * ConnectionNetwork::Deserialize(int buf_size)
{
	auto * new_map = new std::map<string , string>;
	std::stringstream ss;
	ss << _buf_recv;
	boost::archive::text_iarchive iarch(ss);
	iarch >> *new_map;
	return new_map;
};

void ConnectionNetwork::SendMsg(map<string, string> &server_answer)
{
	int msg_size = Serialize(server_answer);
	if(msg_size <= 0)
		return;
	Send(msg_size);
};

map<string, string> * ConnectionNetwork::RecvMsg()
{
	std::size_t read_size = Recv();
	map<string, string> * server_answer = Deserialize(read_size);
	return server_answer;
};

void ConnectionNetwork::SendFile(File &file_obj)
{

};

void ConnectionNetwork::RecvFile(File &file_obj_ptr)
{

};

ConnectionNetwork::~ConnectionNetwork()
{

};