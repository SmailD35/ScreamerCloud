#include <queue>
#include "network_mock.h"
#include "database_mock.h"

enum CmdCodeServer { UPLOAD_SRV, DOWNLOAD_SRV, DELETE_SRV, LIST_SRV, AUTH_SRV, REGISTER_SRV };

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