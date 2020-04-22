#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "classes.h"
#include <fstream>

using namespace std;
using ::testing::Return;

class CommandsTest : public ::testing::Test
{
protected:
	ConnectionNetworkMock connectionNetworkMock;
	DatabaseManagerMock databaseManagerMock;
	map<string,string> client_query;
	UserSession* userSession = nullptr;
	Command* command = nullptr;
	Invoker* invoker = nullptr;

	void SetUp() override
	{
		userSession = new UserSession(&connectionNetworkMock, &databaseManagerMock);
	}

	void TearDown() override
	{
		delete userSession;
		delete invoker;
		delete command;
	}
};

TEST_F(CommandsTest, register_user_test)
{
	command = new RegisterUserCommand(*userSession);

	invoker = new Invoker(command);
	invoker->Do();

	EXPECT_CALL(databaseManagerMock, Register).WillOnce(Return(0));
	EXPECT_CALL(connectionNetworkMock, SendMsg).Times(1);
}

TEST_F(CommandsTest, send_file_test)
{
	command = new SendFileCommand(*userSession);

	invoker = new Invoker(command);
	invoker->Do();

	EXPECT_CALL(databaseManagerMock, DownloadFile).Times(1);
	EXPECT_CALL(connectionNetworkMock, SendMsg).Times(1);
	EXPECT_CALL(connectionNetworkMock, SendFile).Times(1);
}

TEST_F(CommandsTest, recv_file_test)
{
	command = new RecvFileCommand(*userSession);

	invoker = new Invoker(command);
	invoker->Do();

	EXPECT_CALL(databaseManagerMock, UploadFile).WillOnce(Return(0));
	EXPECT_CALL(connectionNetworkMock, SendMsg).Times(1);
	EXPECT_CALL(connectionNetworkMock, RecvFile).Times(1);
}

TEST_F(CommandsTest, send_file_list_test)
{
	command = new SendFileListCommand(*userSession);

	invoker = new Invoker(command);
	invoker->Do();

	EXPECT_CALL(databaseManagerMock, GetFileList).Times(1);
	EXPECT_CALL(connectionNetworkMock, SendMsg).Times(1);
}