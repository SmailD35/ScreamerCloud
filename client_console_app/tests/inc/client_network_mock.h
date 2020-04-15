//
// Created by Aleksandr Dergachev on 13.04.2020.
//

#ifndef CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_MOCK_H_
#define CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_MOCK_H_

#define UPLOAD_SRV 		"1"
#define DOWNLOAD_SRV 	"2"
#define DELETE_SRV 		"3"
#define LIST_SRV 		"4"
#define AUTH_SRV 		"5"

#include <string>
#include <vector>
#include "file.h"
#include "gmock/gmock.h"

using namespace std;

class ClientNetwork
{
 public:
	virtual void SendMsg(map<string,string> client_query) = 0;
	virtual map<string,string> RecvMsg() = 0;
	virtual void SendFile(File file_obj) = 0;
	virtual void RecvFile(File* file_ptr) = 0;
};

class ClientNetworkMock : public ClientNetwork
{
 public:
	MOCK_METHOD(void, SendMsg, ((map<string,string>) client_query), (override));
	MOCK_METHOD((map<string,string>), RecvMsg, (), (override));
	MOCK_METHOD(void, SendFile, (File file_obj), (override));
	MOCK_METHOD(void, RecvFile, (File* file_ptr), (override));
};

#endif //CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_MOCK_H_