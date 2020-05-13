#include "../headers/test_network_client.h"
#include "../../../network_client/headers/network_client.h"

#define SEND_BUF 1
#define RECV_BUF 2


using ::testing::Return;
using ::testing::SetArgPointee;

char * ClientNetworkTest::GetBuf(int choice)
{

	if(choice == 1)
		return buf_send;
	else
		return buf_recv;
}

void ClientNetworkTest::SetBuf(int choice, char * buf)
{
	switch (choice)
	{
	case 1:
		strcpy(buf_send, buf);
		break;

	case 2:
		strcpy(buf_recv, buf);
		break;
	default:
		strcpy(buf_send, buf);
		break;

	}
}

class NetworkTest : public ::testing::Test
{
protected:
	ClientNetworkTest * clientNetwork;
	map<string,string> client_query;
	FileMock * file;

	void SetUp() override
	{
		file = new FileMock();
		clientNetwork = new ClientNetworkTest();
	}
};


TEST_F(NetworkTest, client_send_msg_test)
{
	client_query["cmd_code"] = AUTH_SRV;
	client_query["login"] = "egor";
	client_query["password"] = "121221";

	char * buf = (char *)"504egor06121221";

	clientNetwork->SendMsg(client_query);
	char * test_buf = clientNetwork->GetBuf(SEND_BUF);

	ASSERT_EQ(buf, test_buf);

	free(buf);
}

TEST_F(NetworkTest, client_recv_msg_test)
{
	client_query["cmd_code"] = AUTH_SRV;
	client_query["error_code"] = "2";

	char * buf = (char *)"52";

	clientNetwork->SetBuf(RECV_BUF, buf);
	map<string,string> test_client_query = clientNetwork->RecvMsg();

	ASSERT_EQ(client_query["cmd_code"], test_client_query["cmd_code"]);
	ASSERT_EQ(client_query["error_code"], test_client_query["error_code"]);

	free(buf);
}

TEST_F(NetworkTest, client_send_file_msg_test)
{
	client_query["cmd_code"] = UPLOAD_SRV;
	client_query["login"] = "egor";
	client_query["password"] = "121221";
	client_query["size"] = "1000000";
	client_query["MD5"] = "a932b0e624cb7e62efe0c7d1368dc71f";
	client_query["file_name"] = "test.txt";

	char * buf = (char *)"104egor061212210071000000032a932b0e624cb7e62efe0c7d1368dc71f008test.txt";

	clientNetwork->SendMsg(client_query);
	char * test_buf = clientNetwork->GetBuf(SEND_BUF);

	ASSERT_EQ(buf, test_buf);

	free(buf);
}

TEST_F(NetworkTest, client_recv_file_msg_test)
{
	client_query["cmd_code"] = DOWNLOAD_SRV;
	client_query["size"] = "1000000";
	client_query["MD5"] = "a932b0e624cb7e62efe0c7d1368dc71f";
	client_query["file_name"] = "test.txt";

	char * buf = (char *)"20071000000032a932b0e624cb7e62efe0c7d1368dc71f008test.txt";

	clientNetwork->SetBuf(RECV_BUF, buf);
	map<string,string> test_client_query = clientNetwork->RecvMsg();

	ASSERT_EQ(client_query["cmd_code"], test_client_query["cmd_code"]);
	ASSERT_EQ(client_query["size"], test_client_query["size"]);
	ASSERT_EQ(client_query["MD5"], test_client_query["MD5"]);
	ASSERT_EQ(client_query["file_name"], test_client_query["file_name"]);

	free(buf);
}


TEST_F(NetworkTest, client_recv_file_test)
{
	client_query["cmd_code"] = DOWNLOAD_SRV;
	client_query["size"] = "1000000";
	client_query["MD5"] = "a932b0e624cb7e62efe0c7d1368dc71f";
	client_query["file_name"] = "test.txt";

	char * buf = (char *)"20071000000032a932b0e624cb7e62efe0c7d1368dc71f008test.txt";

	clientNetwork->SetBuf(RECV_BUF, buf);

	clientNetwork->RecvFile(file);

	string fhash = file->GetHash();
	string fsize = to_string(file->GetSize());

	ASSERT_EQ(client_query["size"], fsize);
	ASSERT_EQ(client_query["MD5"], fhash);

	free(buf);
}

