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

	Command();
	explicit Command(UserSession userSession) : _userSession(userSession) {};
    virtual void Do() = 0;
    virtual void Undo() = 0;

};

class Invoker
{
private:

    Command* _command;

public:

	explicit Invoker(Command* command) : _command(command) {};
    void Do();
    void Undo();
};

class RegisterUserCommand : public Command
{
public:

	using Command::Command;
    void Do() override;
    void Undo() override;
};

class SendFileCommand : public Command
{
public:

	using Command::Command;
    void Do() override;
    void Undo() override;
};

class RecvFileCommand : public Command
{
public:

	using Command::Command;
	void Do() override;
    void Undo() override;
};

class SendFileListCommand : public Command
{
public:

	using Command::Command;
	void Do() override;
    void Undo() override;
};

class Server
{
private:
	//указатель потому что тесты
	ServerNetwork* _network;
    queue<ConnectionNetwork> _connections;
    queue<Command> _queries;
    Command* CreateCommand(UserSession userSession);

public:

    void ConnectionsLoop();
    void QueriesLoop();
    void ExecuteCommand();
};