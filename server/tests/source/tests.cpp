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

	void SetUp() override
	{
		userSession = new UserSession(&connectionNetworkMock, &databaseManagerMock);
	}
};

TEST_F(CommandsTest, register_user_test)
{
	client_query["cmd_code"] = REGISTER_SRV;
	client_query["login"] = "vasya";
	client_query["password"] = "12345";

	EXPECT_CALL(connectionNetworkMock, RecvMsg).WillOnce(Return(client_query));
	EXPECT_CALL();

	command = new RegisterUserCommand(*userSession);

}