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

std::size_t ClientNetwork::Send()
{
	return boost::asio::write(*_socket, boost::asio::buffer(buf_send));
};

std::size_t ClientNetwork::Recv()
{
	boost::asio::streambuf buf;
	std::size_t read_size;
	try
	{
		read_size = boost::asio::read_until(*_socket, buf, "\0");
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
	}
	buf_recv = boost::asio::buffer_cast<const char*>(buf.data());
	size_t buf_size = buf_recv.size();
	return buf_size;
};

void ClientNetwork::Serialize(map<string, string> &client_query)
{
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << client_query;
	buf_send = ss.str();
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
	Serialize(client_query);
	Send();
};

map<string, string> * ClientNetwork::RecvMsg()
{
	Recv();
	map<string, string> * server_answer = Deserialize(0);
	return server_answer;
}

ClientNetwork::~ClientNetwork()
= default;

int ClientNetwork::SendFile(InFile * file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t send_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (; send_bytes < file_size;)
	{
		buf = file_obj->GetNextChunk();
		send_bytes += boost::asio::write(*_socket, boost::asio::buffer(buf));
	}
	return 0;
};

int ClientNetwork::RecvFile(OutFile * file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t recv_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (; recv_bytes < file_size;)
	{
		try
		{
			boost::asio::read(*_socket, boost::asio::buffer(buf, chunkSize));
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

char * ClientNetworkTest::GetBuf(int choice) {};

void ClientNetworkTest::SetBuf(int choice, char *buf) {};