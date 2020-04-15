//
// Created by Aleksandr Dergachev on 10.04.2020.
//

#include "client_app.h"

using namespace std;

ClientApp::ClientApp()
{

}

ClientApp::~ClientApp()
{

}

void ClientApp::ParseCmdArguments(int argc, char** argv)
{
	if (argc == 1)
	{
		_clientRequest.cmdCode = HELP_CLI;
		return;
	}

	if (!strncmp("help", argv[1], 4))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.requestData["topic"] = string(argv[2]);
		default:
			_clientRequest.cmdCode = HELP_CLI;
		}
	}

	if (!strncmp("upload", argv[1], 6))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.cmdCode = UPLOAD_CLI;
			_clientRequest.requestData["file_name"] = string(argv[2]);
			break;
		default:
			_clientRequest.cmdCode = HELP_CLI;
			_clientRequest.requestData["topic"] = string(argv[1]);
		}
	}

	if (!strncmp("download", argv[1], 8))
	{
		switch (argc)
		{
		case 4:
			_clientRequest.requestData["download_path"] = string(argv[3]);
		case 3:
			_clientRequest.cmdCode = DOWNLOAD_CLI;
			_clientRequest.requestData["file_name"] = string(argv[2]);
			break;
		default:
			_clientRequest.cmdCode = HELP_CLI;
			_clientRequest.requestData["topic"] = string(argv[1]);
		}
	}

	if (!strncmp("delete", argv[1], 6))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.cmdCode = DELETE_CLI;
			_clientRequest.requestData["file_name"] = string(argv[2]);
			break;
		default:
			_clientRequest.cmdCode = HELP_CLI;
			_clientRequest.requestData["topic"] = string(argv[1]);
		}
	}

	if (!strncmp("list", argv[1], 4))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.requestData["path"] = string(argv[2]);
		case 2:
			_clientRequest.cmdCode = LIST_CLI;
			break;
		default:
			_clientRequest.cmdCode = HELP_CLI;
			_clientRequest.requestData["topic"] = string(argv[1]);
		}
	}

	if (!strncmp("register", argv[1], 8))
	{
		_clientRequest.cmdCode = REGISTER_CLI;
	}

	if (!strncmp("login", argv[1], 5))
	{
		_clientRequest.cmdCode = LOGIN_CLI;
	}
	return;
}

int ClientApp::ExecuteRequest()
{
	switch (_clientRequest.cmdCode)
	{
	case HELP_CLI:
		if(_clientRequest.requestData.count("topic"))
			return Help(_clientRequest.requestData["topic"]);
		else
			return Help();
	case UPLOAD_CLI:
		return UploadFile(_clientRequest.requestData["file_name"]);
	case DOWNLOAD_CLI:
		if(_clientRequest.requestData.count("download_path"))
			return DownloadFile(_clientRequest.requestData["file_name"],
				_clientRequest.requestData["download_path"]);
		else
			return DownloadFile(_clientRequest.requestData["file_name"]);
	case DELETE_CLI:
		return DeleteFile(_clientRequest.requestData["file_name"]);
	case LIST_CLI:
		if(_clientRequest.requestData.count("path"))
			return ListDirectory(_clientRequest.requestData["path"]);
		else
			return ListAll();
	case REGISTER_CLI:
		return RegisterUser();
	case LOGIN_CLI:
		return LoginUser();
	default:
		return -1;
	}
	return 0;
}

int ClientApp::UploadFile(string file_name)
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != UPLOAD_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		_clientNetwork->SendFile(_file);
	else
		return 4;
	return 0;
}

int ClientApp::DownloadFile(string file_name)
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != DOWNLOAD_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		_clientNetwork->RecvFile(&_file);
	else
		return 4;
	return 0;
}

int ClientApp::DownloadFile(string file_name, string download_path)
{
	cout << "Download file: " << file_name << " in " << download_path << " directory\n";
	return 0;
}

int ClientApp::DeleteFile(string file_name)
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != DELETE_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		return 0;
	else
		return 4;
}

int ClientApp::ListAll()
{
	_clientNetwork->SendMsg(_clientRequest.requestData);
	map<string,string> receivedMsg;
	receivedMsg = _clientNetwork->RecvMsg();
	if (!receivedMsg.count("cmd_code"))
		return 1;
	if (receivedMsg["cmd_code"] != LIST_SRV)
		return 2;
	if (!receivedMsg.count("error_code"))
		return 3;
	if (receivedMsg["error_code"] == "0")
		return 0;
	else
		return 4;
}

int ClientApp::ListDirectory(string directory_path)
{
	cout << "List directory: " << directory_path << "\n";
	return 0;
}

int ClientApp::RegisterUser()
{
	cout << "Registration process\n";
	return 0;
}

int ClientApp::LoginUser()
{
	cout << "Login process\n";
	return 0;
}

int ClientApp::Help()
{
	cout << "help screamer\n";
	return 0;
}

int ClientApp::Help(string help_topic)
{
	cout << "help " << help_topic << "\n";
	return 0;
}