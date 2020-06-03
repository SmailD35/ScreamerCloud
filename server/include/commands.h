#include <queue>
#include <utility>
#include "network_server.h"
#include "database_manager.h"
#include "server_logging.hpp"
#include "cmd_codes.h"
#include "error_codes.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

class UserSession
{
public:

	UserSession(const ConnectionNetwork& userConnection,
			std::map<std::string, std::string> userQuery, const std::string& storagePath);
	~UserSession();
    ConnectionNetwork _userConnection;
    DatabaseManager _databaseManager;
    std::map<std::string, std::string> _userQuery;
    std::string _userPath;
    bool _isAuthorized = false;

};

class Command
{
protected:

    UserSession _userSession;

public:

	explicit Command(UserSession userSession) : _userSession(userSession) {};
    virtual int Do() = 0;
    virtual int Undo() = 0;

};

class Invoker
{
private:

    std::shared_ptr<Command> _command;

public:

	explicit Invoker(std::shared_ptr<Command> command) : _command(std::move(command)) {};
    int Do();
    int Undo();
};

class RegisterUserCommand : public Command
{
public:

	using Command::Command;
    int Do() override;
    int Undo() override {};
};

class LoginUserCommand : public Command
{
public:

	using Command::Command;
	int Do() override;
	int Undo() override {};
};

class UploadFileCommand : public Command
{
public:

	using Command::Command;
	int Do() override;
	int Undo() override;
};

class DownloadFileCommand : public Command
{
public:

	using Command::Command;
	int Do() override;
	int Undo() override {};
};

class SendFileListCommand : public Command
{
public:

	using Command::Command;
	int Do() override;
	int Undo() override {};
};

class DeleteFileCommand : public Command
{
public:

	using Command::Command;
	int Do() override;
	int Undo() override {};
};

class DeleteUserCommand : public Command
{
public:

	using Command::Command;
	int Do() override;
	int Undo() override {};
};