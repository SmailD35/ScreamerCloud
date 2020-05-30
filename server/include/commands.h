#include <queue>
#include "network_server.h"
#include "database_manager.h"


class UserSession
{
public:

	//UserSession();
	UserSession(ConnectionNetwork userConnection) : _userConnection(userConnection) {};
	~UserSession();
    ConnectionNetwork _userConnection;
    DatabaseManager _databaseManager;
    std::map<std::string, std::string> _userQuery;

};

class Command
{
protected:

    UserSession _userSession;

public:

	//Command();
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

class LoginUserCommand : public Command
{
public:

	using Command::Command;
	void Do() override;
	void Undo() override;
};

class UploadFileCommand : public Command
{
public:

	using Command::Command;
    void Do() override;
    void Undo() override;
};

class DownloadFileCommand : public Command
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

class DeleteFileCommand : public Command
{
public:

	using Command::Command;
	void Do() override;
	void Undo() override;
};

class DeleteUserCommand : public Command
{
public:

	using Command::Command;
	void Do() override;
	void Undo() override;
};