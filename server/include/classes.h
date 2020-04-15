#include <queue>
#include "network_mock.h"
#include "database_mock.h"

class UserSession
{
public:

    ConnectionNetwork _userConnection;
    DatabaseManager _databaseManager;
    map<string, string> _userQuery;

};

class Command
{
protected:

    UserSession _userSession;

public:

    virtual void Do();
    virtual void Undo();

};

class Invoker
{
private:

    Command _command;

public:

    void Do();
    void Undo();
    void SetCommand(Command command);
};

class RegisterUserCommand : Command
{
public:

    void Do();
    void Undo();
};

class SendFileCommand : Command
{
public:

    void Do();
    void Undo();
};

class RecvFileCommand : Command
{
public:

    void Do();
    void Undo();
};

class SendFileListCommand : Command
{
public:

    void Do();
    void Undo();
};

class Server
{
private:

    ServerNetwork _network;
    queue<ConnectionNetwork> _connections;
    queue<Command> _queries;
    Command CreateCommand(UserSession userSession);

public:

    void ConnectionsLoop();
    void QueriesLoop();
};