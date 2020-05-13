#include "commands.h"

UserSession::UserSession()
{

}

UserSession::~UserSession()
{

}

Command::Command()
{

}

void Invoker::Do()
{

}

void Invoker::Undo()
{

}

void RegisterUserCommand::Do()
{

}

void RegisterUserCommand::Undo()
{

}

void SendFileCommand::Do()
{

}

void SendFileCommand::Undo()
{

}

void RecvFileCommand::Do()
{

}

void RecvFileCommand::Undo()
{

}

void SendFileListCommand::Do()
{

}

void SendFileListCommand::Undo()
{

}

Command* Server::CreateCommand(UserSession userSession)
{
	Command* result = nullptr;
	return result;
}

void Server::ConnectionsLoop()
{

}

void Server::QueriesLoop()
{

}

void Server::ExecuteCommand()
{

}
