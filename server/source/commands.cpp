#include "commands.h"

using namespace std;

UserSession::UserSession(const ConnectionNetwork& userConnection,
		std::map<std::string, std::string> userQuery, const std::string& storagePath)
		: _userConnection(userConnection)
{
	_userQuery = move(userQuery);
	_isAuthorized = _databaseManager.Authorize(_userQuery["username"], _userQuery["password"]);
	if (_isAuthorized)
		_userPath = storagePath + '/' + _databaseManager.GetUserDir();
}

UserSession::~UserSession()
{
	//_userConnection.CloseConnection();
}

int Invoker::Do()
{
	return _command->Do();
}

int Invoker::Undo()
{
	return _command->Undo();
}

int RegisterUserCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	bool dbError = !dbManager.Register(query["username"], query["password"]);
	query["error_code"] = to_string(dbError);

	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in RegisterUserCommand failed";

	return SERVER_NOERROR;
}

int LoginUserCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	query["error_code"] = to_string(!_userSession._isAuthorized);

	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in LoginUserCommand failed";

	return SERVER_NOERROR;
}

int UploadFileCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	if (!_userSession._isAuthorized)
	{
		query["error_code"] = to_string(SERVER_AUTH_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in UploadFileCommand failed";
		return SERVER_AUTH_ERROR;
	}

	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in UploadFileCommand failed";
		return SERVER_UPLOAD_ERROR;
	}

	stringstream sstream(query["file_size"]);
	size_t size = 0;
	sstream >> size;
	auto file = make_shared<OutFile>(size, _userSession._userPath, query["file_name"]);

	netError = network.RecvFile(file);
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " RecvFile in UploadFileCommand failed";
		return SERVER_UPLOAD_ERROR;
	}

	bool dbError = !dbManager.Upload(query["file_name"], query["file_directory"], "1234");
	if (dbError)
	{
		query["error_code"] = to_string(SERVER_UPLOAD_ERROR);
		netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in UploadFileCommand failed";
		return SERVER_UPLOAD_ERROR;
	}

	netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in UploadFileCommand failed";


	return SERVER_NOERROR;
	//TODO: проверить хеш-сумму
}

int UploadFileCommand::Undo()
{
	fs::remove(_userSession._userPath + '/' + _userSession._userQuery["file_name"]);
	return SERVER_NOERROR;
}

int DownloadFileCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	if (!_userSession._isAuthorized)
	{
		query["error_code"] = to_string(SERVER_AUTH_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DownloadFileCommand failed";
		return SERVER_AUTH_ERROR;
	}

	shared_ptr<InFile> file = dbManager.Download(query["file_name"], query["file_directory"]);
	if (file == nullptr)
	{
		query["error_code"] = to_string(SERVER_DOWNLOAD_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DownloadFileCommand failed";
		return SERVER_DOWNLOAD_ERROR;
	}
	query["file_size"] = to_string(file->GetSize());

	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DownloadFileCommand failed";
		return SERVER_DOWNLOAD_ERROR;
	}

	netError = network.SendFile(file);
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendFile in DownloadFileCommand failed";
		return SERVER_DOWNLOAD_ERROR;
	}

	return SERVER_NOERROR;
}

int SendFileListCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	if (!_userSession._isAuthorized)
	{
		query["error_code"] = to_string(SERVER_AUTH_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in SendFileListCommand failed";
		return SERVER_AUTH_ERROR;
	}

	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in SendFileListCommand failed";
		return SERVER_LIST_ERROR;
	}

	map<string,string> list = dbManager.GetFileList(query["directory"]);
	netError = network.SendMsg(make_shared<map<string, string>>(list));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in SendFileListCommand failed";
		return SERVER_LIST_ERROR;
	}

	return SERVER_NOERROR;
}

int DeleteFileCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	if (!_userSession._isAuthorized)
	{
		query["error_code"] = to_string(SERVER_AUTH_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DeleteFileCommand failed";
		return SERVER_AUTH_ERROR;
	}

	int dbError = !dbManager.DeleteFile(query["file_name"], query["file_directory"]);
	query["error_code"] = to_string(dbError);
	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DeleteFileCommand failed";
		return SERVER_DELETEFILE_ERROR;
	}

	return SERVER_NOERROR;
}

int DeleteUserCommand::Do()
{
	DatabaseManager &dbManager = _userSession._databaseManager;
	ConnectionNetwork &network = _userSession._userConnection;
	map<string,string> &query = _userSession._userQuery;

	if (!_userSession._isAuthorized)
	{
		query["error_code"] = to_string(SERVER_AUTH_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DeleteUserCommand failed";
		return SERVER_AUTH_ERROR;
	}

	int dbError = !dbManager.DeleteUser(query["username"], query["password"]);
	query["error_code"] = to_string(dbError);
	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DeleteUserCommand failed";
		return SERVER_DELETEUSER_ERROR;
	}

	return SERVER_NOERROR;
}