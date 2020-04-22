#ifndef NETWORK_TEST_NETWORK_SERVER_H
#define NETWORK_TEST_NETWORK_SERVER_H

#include "gmock/gmock.h"
#include <string>
#include "test_network_client.h"
#include "gtest/gtest.h"
#include "../../headers/network_client.h"
#include "../../headers/network_server.h"
#include <cstring>

using namespace std;


class ConnectionNetworkTest : public ConnectionNetwork
{
public:
	char * GetBuf(int choice);
	void SetBuf(int choice, char * buf);
};


#endif //NETWORK_TEST_NETWORK_SERVER_H
