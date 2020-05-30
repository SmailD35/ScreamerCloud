#include "network_server.h"

#include <utility>

namespace io = boost::asio;
using namespace std;


ServerNetwork::ServerNetwork(const string& IP, int port) : _acceptor(_io_service,
		io::ip::tcp::endpoint (io::ip::address::from_string(IP), port))
{
	_IP = IP;
	_port = port;
};

ConnectionNetwork ServerNetwork::StandConnection()
{
	boost::shared_ptr<boost::asio::ip::tcp::socket> sock  = boost::make_shared<io::ip::tcp::socket>(_io_service);
	_acceptor.accept(*sock);
	ConnectionNetwork client_connect(sock);
	return client_connect;
}

ServerNetwork::~ServerNetwork()
= default;

ConnectionNetwork::ConnectionNetwork(boost::shared_ptr<boost::asio::ip::tcp::socket> sock)
{
	socket = std::move(sock);
};

int ConnectionNetwork::Send()
{
	int read_bytes = 0;
	try
	{
		read_bytes = boost::asio::write(*socket, boost::asio::buffer(_buf_send));
	}
	catch (const boost::system::system_error& e)
	{
		cout << e.what() << endl;
		return ERROR_SEND;
	}
	return read_bytes;
};

int ConnectionNetwork::Recv()
{
	boost::asio::streambuf buf;
	std::size_t read_size;
	try
	{
		read_size = boost::asio::read_until(*socket, buf, "\0");
	}
	catch(const boost::system::system_error& e)
	{
		cout << e.what() << endl;
		return ERROR_RECV;
	}
	_buf_recv = boost::asio::buffer_cast<const char*>(buf.data());
	return read_size;
};

void ConnectionNetwork::Serialize(const std::shared_ptr<map<string, string>> &server_answer)
{
	std::stringstream ss;
	boost::archive::text_oarchive oarch(ss);
	oarch << *server_answer;
	_buf_send = ss.str();
};

std::shared_ptr<map<string, string>> ConnectionNetwork::Deserialize()
{
	auto new_map = std::make_shared<map<string, string>>();
	std::stringstream ss;
	ss << _buf_recv;
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

int ConnectionNetwork::SendMsg(const std::shared_ptr<map<string, string>> &server_answer)
{
	if (server_answer->empty())
		return ERROR_INPUT_MAP;
	Serialize(server_answer);
	if (Send() < 0)
		return ERROR_SEND;
	return 0;
};

std::shared_ptr<map<string, string>> ConnectionNetwork::RecvMsg()
{
	if (Recv() < 0)
	{
		return nullptr;
	}
	std::shared_ptr<map<string, string>> server_answer = Deserialize();
	return server_answer;
}

int ConnectionNetwork::SendFile(const std::shared_ptr<InFile>& file_obj)
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
			send_bytes += boost::asio::write(*socket, boost::asio::buffer(buf));
		}
		catch (const boost::system::system_error& e)
		{
			cout << e.what() << endl;
			return ERROR_SEND;
		}
	}
	return 0;
};

int ConnectionNetwork::RecvFile(const std::shared_ptr<OutFile>& file_obj)
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
			return ERROR_RECV;
		}
		recv_bytes += buf.size();

		file_obj->SetNextChunk(buf);
	}
	return 0;
};

ConnectionNetwork::~ConnectionNetwork()
{
	socket->close();
};