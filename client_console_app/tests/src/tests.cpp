//
// Created by Aleksandr Dergachev on 14.04.2020.
//

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "client_network_mock.h"
#include "client_app.h"

using ::testing::Return;
using ::testing::SetArgPointee;

struct TestData
{
	string arguments;

};

TEST(ClientTest, InteractsWithNetwork)
{
	ClientNetworkMock clientNetworkMock;
	map<string,string> client_query[5];

	client_query[0]["cmd_code"] = UPLOAD_SRV;
	client_query[0]["error_code"] = "0";

	client_query[1]["cmd_code"] = DOWNLOAD_SRV;
	client_query[1]["error_code"] = "0";
	client_query[1]["size"] = "1000000";
	client_query[1]["MD5"] = "a932b0e624cb7e62efe0c7d1368dc71f";
	client_query[1]["file_name"] = "privet.txt";

	client_query[2]["cmd_code"] = DELETE_SRV;
	client_query[2]["error_code"] = "0";

	client_query[3]["cmd_code"] = LIST_SRV;
	client_query[3]["error_code"] = "0";
	client_query[3]["files_buf"] = "";

	client_query[4]["cmd_code"] = AUTH_SRV;
	client_query[4]["error_code"] = "0";

	EXPECT_CALL(clientNetworkMock, RecvMsg())
	.WillOnce(Return(client_query[0]));
//	.WillOnce(Return(client_query[1]))
//	.WillOnce(Return(client_query[2]))
//	.WillOnce(Return(client_query[3]))
//	.WillOnce(Return(client_query[4]));

	File downloadedFile;
	EXPECT_CALL(clientNetworkMock, RecvFile)
	.WillRepeatedly(SetArgPointee<0>(downloadedFile));

	EXPECT_CALL(clientNetworkMock, SendFile).Times(1);
	EXPECT_CALL(clientNetworkMock, SendMsg).Times(1);


	ClientApp clientAppTest(&clientNetworkMock);
	char** arguments;
	arguments = new char*[3];
	arguments[0] = new char[8];
	arguments[0] = "screamer";
	arguments[1] = new char[6];
	arguments[1] = "upload";
	arguments[2] = new char[9];
	arguments[2] = "petya.txt";
	clientAppTest.ParseCmdArguments(3, arguments);
	int error = clientAppTest.ExecuteRequest();
}