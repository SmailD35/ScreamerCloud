#include "commands.h"

using namespace std;

UserSession::~UserSession()
{

}

//Command::Command()
//{
//
//}

void Invoker::Do()
{
	_command->Do();
}

void Invoker::Undo()
{
	_command->Undo();
}

void RegisterUserCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool errorCode = dbManager.Register(query["username"], query["password"]);
	query["error_code"] = to_string(errorCode);

	network.SendMsg(query);
}

void RegisterUserCommand::Undo()
{

}

void LoginUserCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool errorCode = dbManager.Authorize(query["username"], query["password"]);
	query["error_code"] = to_string(errorCode);

	network.SendMsg(query);
}

void LoginUserCommand::Undo()
{

}

void SendFileCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool error = dbManager.Authorize(query["username"], query["password"]);
	query["error_code"] = to_string(error);

	network.SendMsg(query);

	if (error == 0)
	{
		string file_path = dbManager.GetUserDir() + query["file_name"];
		auto file = new OutFile(stoi(query["file_size"]), file_path);
		network.RecvFile(reinterpret_cast<OutFile&>(file));
		error = dbManager.Upload(query["file_name"], query["upload_directory"], "");
	}

	network.SendMsg(query);
	//TODO: проверить хеш-сумму
}

void SendFileCommand::Undo()
{

}

void RecvFileCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	shared_ptr<FILE> filePtr = dbManager.Download(query["file_name"], query["directory"]);
	query["error_code"] = "0";
	network.SendMsg(query);
	//network.SendFile(filePtr);
}

void RecvFileCommand::Undo()
{

}

void SendFileListCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	map<string,string> list = dbManager.GetFileList(query["directory"]);
	//TODO: перезапись из вектора в map, или переделать функцию чтобы возвращала map
	network.SendMsg(query);
}

void SendFileListCommand::Undo()
{

}

void DeleteCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool errorCode = dbManager.DeleteFile(query["file_name"], query["directory"]);
	query["error_code"] = std::to_string(errorCode);
	network.SendMsg(query);
}

void DeleteCommand::Undo()
{

}