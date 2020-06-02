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

	bool errorCode = !dbManager.Register(query["username"], query["password"], <#initializer#>);
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

	bool errorCode = !dbManager.Authorize(query["username"], query["password"], <#initializer#>);
	query["error_code"] = to_string(errorCode);

	network.SendMsg(query);
}

void LoginUserCommand::Undo()
{

}

void UploadFileCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool error = !dbManager.Authorize(query["username"], query["password"], <#initializer#>);
	query["error_code"] = to_string(error);

	network.SendMsg(query);

	if (error == 0)
	{
		string file_path = "test_dir/" + dbManager.GetUserDir();
		stringstream sstream(query["file_size"]);
		size_t size = 0;
		sstream >> size;
		auto file = new OutFile(size, file_path, query["file_name"]);
		network.RecvFile(file);
		delete file;
		error = dbManager.Upload(query["file_name"], query["file_directory"], "1234");
	}

	//network.SendMsg(query);
	//TODO: проверить хеш-сумму
}

void UploadFileCommand::Undo()
{

}

void DownloadFileCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool error = !dbManager.Authorize(query["username"], query["password"], <#initializer#>);
	shared_ptr<InFile> file = dbManager.Download(query["file_name"], query["file_directory"]);
	query["file_size"] = to_string(file->GetSize());
	query["error_code"] = to_string(error);

	network.SendMsg(query);

	if (error == 0)
	{
		network.SendFile(file.get());
	}

	//network.SendMsg(query);
}

void DownloadFileCommand::Undo()
{

}

void SendFileListCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool error = !dbManager.Authorize(query["username"], query["password"], <#initializer#>);
	query["error_code"] = to_string(error);
	network.SendMsg(query);

	if (error == 0)
	{
		map<string,string> list = dbManager.GetFileList(query["directory"]);
		network.SendMsg(list);
	}
}

void SendFileListCommand::Undo()
{

}

void DeleteFileCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool error = !dbManager.Authorize(query["username"], query["password"], <#initializer#>);
	if (error == 0)
	{
		error = !dbManager.DeleteFile(query["file_name"], query["file_directory"]);
	}
	query["error_code"] = to_string(error);
	network.SendMsg(query);
}

void DeleteFileCommand::Undo()
{

}

void DeleteUserCommand::Do()
{
	DatabaseManager dbManager = _userSession._databaseManager;
	ConnectionNetwork network = _userSession._userConnection;
	map<string,string> query = _userSession._userQuery;

	bool error = !dbManager.Authorize(query["username"], query["password"], <#initializer#>);
	if (error == 0)
	{
		error = !dbManager.DeleteUser(query["username"], query["password"]);
	}
	query["error_code"] = to_string(error);
	network.SendMsg(query);
}

void DeleteUserCommand::Undo()
{

}