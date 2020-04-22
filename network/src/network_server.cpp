#include "../headers/network_server.h"


ServerNetwork::ServerNetwork()
{

};

ConnectionNetwork ServerNetwork::StandConnection()
{

};


ConnectionNetwork::ConnectionNetwork()
{

};

void ConnectionNetwork::Send(int buf_size)
{

};

void ConnectionNetwork::Recv(int buf_size)
{

};

int ConnectionNetwork::Serialize()
{

};

map<string, string> ConnectionNetwork::Deserialize(int buf_size)
{

};

void ConnectionNetwork::SendMsg(map<string, string> client_query)
{

};

map<string, string> ConnectionNetwork::RecvMsg()
{

};

void ConnectionNetwork::SendFile(File * file_obj)
{

};

void ConnectionNetwork::RecvFile(File * file_obj_ptr)
{

}

ConnectionNetwork::~ConnectionNetwork()
{

};
