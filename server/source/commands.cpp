#include "commands.h"

using namespace std;

UserSession::UserSession(const ConnectionNetwork& userConnection,
		std::map<std::string, std::string> userQuery, const std::string& storagePath)
		: _userConnection(userConnection)
{
	_userQuery = move(userQuery);
	DbErrorCodes dbError = DB_NOERROR;
	_isAuthorized = _databaseManager.Authorize(_userQuery["username"], _userQuery["password"], dbError);
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

	DbErrorCodes dbError = DB_NOERROR;
	dbManager.Register(query["username"], query["password"], dbError);
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

	if (file->GetHash() != query["hash_sum"])
		query["error_code"] = to_string(SERVER_HASHSUM_ERROR);
	else
	{
		DbErrorCodes dbError = DB_NOERROR;
		dbManager.Upload(query["file_name"], query["file_directory"], query["hash_sum"], dbError);
		if (dbError != DB_NOERROR)
		{
			query["error_code"] = to_string(SERVER_UPLOAD_ERROR);
			netError = network.SendMsg(make_shared<map<string, string>>(query));
			if (netError)
				BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in UploadFileCommand failed";
			return SERVER_UPLOAD_ERROR;
		}
	}

	netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in UploadFileCommand failed";


	return SERVER_NOERROR;
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

	DbErrorCodes dbError = DB_NOERROR;
	shared_ptr<InFile> file = dbManager.Download(query["file_name"], query["file_directory"], dbError);
	if (dbError != DB_NOERROR)
	{
		query["error_code"] = to_string(SERVER_DOWNLOAD_ERROR);
		int netError = network.SendMsg(make_shared<map<string, string>>(query));
		if (netError)
			BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DownloadFileCommand failed";
		return SERVER_DOWNLOAD_ERROR;
	}
	query["hash_sum"] = file->GetHash();
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

	DbErrorCodes dbError = DB_NOERROR;
	map<string,string> list = dbManager.GetFileList(query["directory"], dbError);
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

	DbErrorCodes dbError = DB_NOERROR;
	dbManager.DeleteFile(query["file_name"], query["file_directory"], dbError);
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

	DbErrorCodes dbError = DB_NOERROR;
	dbManager.DeleteUser(query["username"], query["password"], dbError);
	query["error_code"] = to_string(dbError);
	int netError = network.SendMsg(make_shared<map<string, string>>(query));
	if (netError)
	{
		BOOST_LOG_TRIVIAL(error) << network.GetClientIP() << " SendMsg in DeleteUserCommand failed";
		return SERVER_DELETEUSER_ERROR;
	}

	return SERVER_NOERROR;
}