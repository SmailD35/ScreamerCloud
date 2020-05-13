#ifndef SERVER_NETWORK_MOCK_H
#define SERVER_NETWORK_MOCK_H

#include <map>
#include <string>
#include "gmock/gmock.h"

using namespace std;

class File
{
public:

    virtual int GetSize() = 0;
    virtual string GetHash() = 0;
};

class ConnectionNetwork
{
public:

    virtual void SendMsg() = 0;
    virtual map<string, string> RecvMsg() = 0;
    virtual void SendFile(File* file_obj) = 0;
    virtual void RecvFile(File* file_ptr) = 0;
};

class ServerNetwork
{
public:

    virtual ConnectionNetwork* StandConnection() = 0;
};


class ServerNetworkMock : public ServerNetwork
{
public:

    MOCK_METHOD(ConnectionNetwork*, StandConnection, (), (override));
};


class ConnectionNetworkMock : public ConnectionNetwork
{
public:

    MOCK_METHOD(void, SendMsg, (), (override));
    MOCK_METHOD((map<string, string>), RecvMsg, (), (override));
    MOCK_METHOD(void, SendFile, (File* file_obj), (override));
    MOCK_METHOD(void, RecvFile, (File* file_ptr), (override));

};


#endif //SERVER_NETWORK_MOCK_H
