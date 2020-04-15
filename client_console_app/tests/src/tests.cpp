//
// Created by Aleksandr Dergachev on 14.04.2020.
//

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "client_network_mock.h"
#include "client_app.h"

using ::testing::Return;
using ::testing::SetArgPointee;

class ClientTest : public ::testing::Test
{
 protected:
	ClientNetworkMock clientNetworkMock;
	ClientApp* clientAppTest;
	map<string,string> client_query;
	File file;
	char** arguments;
	int error;

	void SetUp() override
	{
		error = 0;
		clientAppTest = new ClientApp(&clientNetworkMock);
	}
};

TEST_F(ClientTest, upload_file_test)
{
	client_query["cmd_code"] = UPLOAD_SRV;
	client_query["error_code"] = "0";

	EXPECT_CALL(clientNetworkMock, SendMsg).Times(1);
	EXPECT_CALL(clientNetworkMock, RecvMsg())
		.WillOnce(Return(client_query));

	EXPECT_CALL(clientNetworkMock, SendFile).Times(1);


	arguments = new char*[3];
	arguments[0] = (char*)"screamer";
	arguments[1] = (char*)"upload";
	arguments[2] = (char*)"petya.txt";
	clientAppTest->ParseCmdArguments(3, arguments);
	error = clientAppTest->ExecuteRequest();
	ASSERT_EQ(error, 0);
}

TEST_F(ClientTest, download_file_test)
{
	client_query["cmd_code"] = DOWNLOAD_SRV;
	client_query["error_code"] = "0";
	client_query["size"] = "1000000";
	client_query["MD5"] = "a932b0e624cb7e62efe0c7d1368dc71f";
	client_query["file_name"] = "privet.txt";

	EXPECT_CALL(clientNetworkMock, SendMsg).Times(1);
	EXPECT_CALL(clientNetworkMock, RecvMsg())
		.WillOnce(Return(client_query));

	EXPECT_CALL(clientNetworkMock, RecvFile).Times(1);


	arguments = new char*[3];
	arguments[0] = (char*)"screamer";
	arguments[1] = (char*)"download";
	arguments[2] = (char*)"petya.txt";
	clientAppTest->ParseCmdArguments(3, arguments);
	error = clientAppTest->ExecuteRequest();
	ASSERT_EQ(error, 0);
}

TEST_F(ClientTest, delete_file_test)
{
	client_query["cmd_code"] = DELETE_SRV;
	client_query["error_code"] = "0";

	EXPECT_CALL(clientNetworkMock, SendMsg).Times(1);
	EXPECT_CALL(clientNetworkMock, RecvMsg())
		.WillOnce(Return(client_query));


	arguments = new char*[3];
	arguments[0] = (char*)"screamer";
	arguments[1] = (char*)"delete";
	arguments[2] = (char*)"petya.txt";
	clientAppTest->ParseCmdArguments(3, arguments);
	error = clientAppTest->ExecuteRequest();
	ASSERT_EQ(error, 0);
}

TEST_F(ClientTest, list_all_test)
{
	client_query["cmd_code"] = LIST_SRV;
	client_query["error_code"] = "0";

	EXPECT_CALL(clientNetworkMock, SendMsg).Times(1);
	EXPECT_CALL(clientNetworkMock, RecvMsg())
		.WillOnce(Return(client_query));


	arguments = new char*[2];
	arguments[0] = (char*)"screamer";
	arguments[1] = (char*)"list";
	clientAppTest->ParseCmdArguments(2, arguments);
	error = clientAppTest->ExecuteRequest();
	ASSERT_EQ(error, 0);
}

//TEST(ClientTest, upload_test)
//{
//	ClientNetworkMock clientNetworkMock;
//	map<string,string> client_query[5];
//
//	client_query[0]["cmd_code"] = UPLOAD_SRV;
//	client_query[0]["error_code"] = "0";
//
//	client_query[1]["cmd_code"] = DOWNLOAD_SRV;
//	client_query[1]["error_code"] = "0";
//	client_query[1]["size"] = "1000000";
//	client_query[1]["MD5"] = "a932b0e624cb7e62efe0c7d1368dc71f";
//	client_query[1]["file_name"] = "privet.txt";
//
//	client_query[2]["cmd_code"] = DELETE_SRV;
//	client_query[2]["error_code"] = "0";
//
//	client_query[3]["cmd_code"] = LIST_SRV;
//	client_query[3]["error_code"] = "0";
//	client_query[3]["files_buf"] = "";
//
//	client_query[4]["cmd_code"] = AUTH_SRV;
//	client_query[4]["error_code"] = "0";
//
//	EXPECT_CALL(clientNetworkMock, RecvMsg())
//	.WillOnce(Return(client_query[0]));
////	.WillOnce(Return(client_query[1]))
////	.WillOnce(Return(client_query[2]))
////	.WillOnce(Return(client_query[3]))
////	.WillOnce(Return(client_query[4]));
//
//	File downloadedFile;
//	EXPECT_CALL(clientNetworkMock, RecvFile)
//	.WillRepeatedly(SetArgPointee<0>(downloadedFile));
//
//	EXPECT_CALL(clientNetworkMock, SendFile).Times(1);
//	EXPECT_CALL(clientNetworkMock, SendMsg).Times(1);
//
//
//	ClientApp clientAppTest(&clientNetworkMock);
//	char** arguments;
//	arguments = new char*[3];
//	arguments[0] = new char[8];
//	arguments[0] = "screamer";
//	arguments[1] = new char[6];
//	arguments[1] = "upload";
//	arguments[2] = new char[9];
//	arguments[2] = "petya.txt";
//	clientAppTest.ParseCmdArguments(3, arguments);
//	int error = clientAppTest.ExecuteRequest();
//
//}