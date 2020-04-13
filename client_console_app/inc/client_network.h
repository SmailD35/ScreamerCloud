//
// Created by Aleksandr Dergachev on 13.04.2020.
//

#ifndef CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_H_
#define CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_H_

#include <string>
#include <vector>
#include "file.h"

using namespace std;

class ClientNetwork
{
	virtual void SendMsg(vector<string> client_query) = 0;
	virtual vector<string> RecvMsg() = 0;
	virtual void SendFile(File file_obj, int file_size) = 0;
	virtual void RecvFile(File* file_ptr) = 0;
};

#endif //CLIENT_CONSOLE_APP_TESTS_CLIENT_NETWORK_H_