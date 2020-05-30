#include "network_client.h"

namespace io = boost::asio;
using namespace std;

ClientNetwork::ClientNetwork(const string& IP, int port) : _socket(_io_service)
{
	_IP = IP;
	_port = port;
};

ClientNetwork::~ClientNetwork()
{
	_socket.close();
};

int ClientNetwork::Connection()
{
	io::ip::tcp::endpoint ep(io::ip::address::from_string(_IP), _port);
	try
	{
		_socket.connect(ep);
	}
	catch (const boost::system::system_error& e)
	{
		cout << e.what() << endl;
		return ERROR_CONNECTION;
	}
	return 0;
};

int ClientNetwork::Send()
{
	int read_bytes = 0;
	try
	{
		read_bytes = boost::asio::write(_socket, boost::asio::buffer(buf_send));
	}
	catch (const boost::system::system_error& e)
	{
		cout << e.what() << endl;
		return ERROR_SEND;
	}
	return read_bytes;
};

int ClientNetwork::Recv()
{
	boost::asio::streambuf buf;
	int read_size = 0;
	try
	{
		read_size = boost::asio::read_until(_socket, buf, "\0");
	}
	catch(std::exception& e)
	{
		cout << e.what() << endl;
		return ERROR_RECV;
	}
	buf_recv = boost::asio::buffer_cast<const char*>(buf.data());
	return read_size;
};

void ClientNetwork::Serialize(map<string, string> &client_query)
{
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << client_query;
	buf_send = ss.str();
};

std::shared_ptr<map<string, string>> ClientNetwork::Deserialize()
{
	auto new_map = std::make_shared<map<string, string>>();
	std::stringstream ss;
	ss << buf_recv;
	try
	{
		boost::archive::text_iarchive iarch(ss);
		iarch >> *new_map;
	}
	catch (std::exception& e)
	{
		cout << e.what() << endl;
		return nullptr;
	}
	return new_map;
};

int ClientNetwork::SendMsg(map<string, string> &client_query)
{
	if (client_query.empty())
		return ERROR_INPUT_MAP;
	Serialize(client_query);
	if (Send() < 0)
		return ERROR_SEND;
	return 0;
};

std::shared_ptr<map<string, string>> ClientNetwork::RecvMsg()
{
	if (Recv() < 0)
	{
		return nullptr;
	}
	std::shared_ptr<map<string, string>> server_answer = Deserialize();
	return server_answer;
}

int ClientNetwork::SendFile(const std::shared_ptr<InFile>& file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t send_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (; send_bytes < file_size;)
	{
		buf = file_obj->GetNextChunk();
		try
		{
			send_bytes += boost::asio::write(_socket, boost::asio::buffer(buf));
		}
		catch (const boost::system::system_error& e)
		{
			cout << e.what() << endl;
			return ERROR_SEND;
		}
	}
	return 0;
};

int ClientNetwork::RecvFile(const std::shared_ptr<OutFile>& file_obj)
{
	size_t file_size = file_obj->GetSize();
	size_t recv_bytes = 0;
	std::array<char, chunkSize> buf;
	buf.fill('\0');

	for (; recv_bytes < file_size;)
	{
		try
		{
			boost::asio::read(_socket, boost::asio::buffer(buf, chunkSize));
		}
		catch(std::exception& e)
		{
			cout << e.what() << endl;
			return ERROR_RECV;
		}
		recv_bytes += buf.size();

		file_obj->SetNextChunk(buf);
	}
	return 0;
};

string ClientNetworkTest::GetBuf(int choice) {};

void ClientNetworkTest::SetBuf(int choice, string buf) {};