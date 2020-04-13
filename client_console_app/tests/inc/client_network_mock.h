//
// Created by Aleksandr Dergachev on 13.04.2020.
//

#ifndef CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_MOCK_H_
#define CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_MOCK_H_

#include <string>
#include <vector>
#include "gmock/gmock.h"
#include "client_network.h"

using namespace std;

class ClientNetworkMock : public ClientNetwork
{
 public:
	MOCK_METHOD(void, SendMsg, (vector<string> client_query), (override));
	MOCK_METHOD(vector<string>, RecvMsg, (), (override));
	MOCK_METHOD(void, SendFile, (File file_obj, int file_size), (override));
	MOCK_METHOD(void, RecvFile, (File* file_ptr), (override));
};

#endif //CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_MOCK_H_