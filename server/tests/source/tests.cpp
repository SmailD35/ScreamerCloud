#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "classes.h"
#include <fstream>

using namespace std;

TEST(server_network_test, stand_connection_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: ServerNetwork::StandConnection();");
}

TEST(server_test, add_to_connections_queue_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: Server::AddToConnectionsQueue();");
}

TEST(server_test, create_command_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: Server::CreateCommand();");
}

TEST(server_test, add_to_command_queue_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: Server::AddToCommandQueue();");
}

TEST(invoker_test, set_command_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: Invoker::SetCommand();");
}

TEST(invoker_test, do_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: Invoker::Do();");
}

TEST(register_user_command_test, do_test)
{
    Server server;
    server.ConnectionsLoop();
    server.QueriesLoop();

    ifstream log_file;
    log_file.open("../logs/log.txt");

    string log_str;
    while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: RegisterUserCommand::Do();");
}

TEST(send_file_command_test, do_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: SendFileCommand::Do();");
}

TEST(recv_file_command_test, do_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: RecvFileCommand::Do();");
}

TEST(send_file_list_command_test, do_test)
{
	Server server;
	server.ConnectionsLoop();
	server.QueriesLoop();

	ifstream log_file;
	log_file.open("../logs/log.txt");

	string log_str;
	while (getline(log_file,log_str))
		ASSERT_NE(log_str, "ERROR: SendFileListCommand::Do();");
}

