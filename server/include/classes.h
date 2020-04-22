#include <queue>
#include "network_mock.h"
#include "database_mock.h"

#define UPLOAD_SRV 		"1"
#define DOWNLOAD_SRV 	"2"
#define DELETE_SRV 		"3"
#define LIST_SRV 		"4"
#define AUTH_SRV 		"5"
#define REGISTER_SRV 	"6"

class UserSession
{
public:

	UserSession();
	UserSession(ConnectionNetwork* userConnection, DatabaseManager* databaseManager)
		: _userConnection(userConnection), _databaseManager(databaseManager) {};
	~UserSession();
	//указатель потому что тесты
    ConnectionNetwork* _userConnection;
    DatabaseManager* _databaseManager;
    map<string, string> _userQuery;

};

class Command
{
protected:

    UserSession _userSession;

public:

	explicit Command(UserSession userSession) : _userSession(userSession) {};
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

class RegisterUserCommand : public Command
{
public:
	using Command::Command;
    void Do();
    void Undo();
};

class SendFileCommand : public Command
{
public:

    void Do();
    void Undo();
};

class RecvFileCommand : public Command
{
public:

    void Do();
    void Undo();
};

class SendFileListCommand : public Command
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
    void ExecuteCommand();
};