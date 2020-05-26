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

int ClientNetwork::SendFile(InFile * file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t send_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (int i = 0; send_bytes < file_size; ++i)
	{
		buf = file_obj->GetNextChunk();
		/*if (buf.empty())
			return 1;*/
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

	for (int i = 0; recv_bytes < file_size; ++i)
	{
		try
		{
			boost::asio::read(*_socket, boost::asio::buffer(buf, chunkSize));
		}
		catch(std::exception& e)
		{
			cout << e.what() << endl;
		}
		//memcpy(&buf[0], boost::asio::buffer_cast<const void*>(buf_s.data()), buf_s.size());
		recv_bytes += buf.size();

		file_obj->SetNextChunk(buf);
	}
	return 0;
};

char * ClientNetworkTest::GetBuf(int choice) {};

void ClientNetworkTest::SetBuf(int choice, char *buf) {};