#include "network_server.h"

namespace io = boost::asio;
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
	io::ip::tcp::endpoint ep(io::ip::address::from_string(_IP), _port);
	_acceptor = new io::ip::tcp::acceptor(_io_service, ep);
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

size_t ConnectionNetwork::Send()
{
	return boost::asio::write(*socket, boost::asio::buffer(_buf_send));
};

size_t ConnectionNetwork::Recv()
{
	boost::asio::streambuf buf;
	std::size_t read_size;
	try
	{
		read_size = boost::asio::read_until(*socket, buf, "\0");
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
	}
	_buf_recv = boost::asio::buffer_cast<const char*>(buf.data());
	size_t buf_size = _buf_recv.size();
	return buf_size;
};

void ConnectionNetwork::Serialize(map<string, string> &server_answer)
{
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << server_answer;
	_buf_send = ss.str();
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
	Serialize(server_answer);
	Send();
};

map<string, string> * ConnectionNetwork::RecvMsg()
{
	std::size_t read_size = Recv();
	map<string, string> * server_answer = Deserialize(read_size);
	return server_answer;
};

int ConnectionNetwork::SendFile(InFile * file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t send_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (; send_bytes < file_size;)
	{
		buf = file_obj->GetNextChunk();
		send_bytes += boost::asio::write(*socket, boost::asio::buffer(buf));
	}
	return 0;
};

int ConnectionNetwork::RecvFile(OutFile * file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t recv_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (; recv_bytes < file_size;)
	{
		try
		{
			boost::asio::read(*socket, boost::asio::buffer(buf, chunkSize));
		}
		catch(std::exception& e)
		{
			cout << e.what() << endl;
		}
		recv_bytes += buf.size();

		file_obj->SetNextChunk(buf);
	}
	return 0;
};

ConnectionNetwork::~ConnectionNetwork()
{

};