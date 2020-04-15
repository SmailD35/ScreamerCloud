#include "../headers/test.h"
#include "gtest/gtest.h"
#include "../../headers/network_client.h"
#include "../../headers/network_server.h"

TEST(test_client_network, send_msg_test)
{
	ClientNetwork clientNetwork;

	clientNetwork.SendMsg();


	ASSERT_EQ();
}

TEST(test_client_network, recv_msg_test)
{
	ClientNetwork clientNetwork;

	clientNetwork.RecvMsg()

	ASSERT_EQ();
}

TEST(test_client_network, send_file_test)
{
	ClientNetwork clientNetwork;

	clientNetwork.SendFile();


	ASSERT_EQ();
}

TEST(test_client_network, recv_file_test)
{
	ClientNetwork clientNetwork;

	clientNetwork.RecvFile();


	ASSERT_EQ();
}


TEST(test_connection_network, send_msg_test)
{
	ConnectionNetwork connectionNetwork;

	connectionNetwork.SendMsg();


	ASSERT_EQ();
}

TEST(test_connection_network, recv_msg_test)
{
	ConnectionNetwork connectionNetwork;

	connectionNetwork.RecvMsg();

	ASSERT_EQ();
}

TEST(test_connection_network, send_file_test)
{
	ConnectionNetwork connectionNetwork;

	connectionNetwork.SendFile();


	ASSERT_EQ();
}

TEST(test_connection_network, recv_file_test)
{
	ConnectionNetwork connectionNetwork;

	connectionNetwork.RecvFile();


	ASSERT_EQ();
}

TEST(test_server_network, stand_connection_test)
{
	ServerNetwork serverNetwork;

	serverNetwork.StandConnection();

	ASSERT_EQ();
}
