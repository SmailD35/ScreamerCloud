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
		_clientRequest.cmdCode = HELP;
		return;
	}

	if (!strncmp("help", argv[1], 4))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.requestData.insert( pair<string,string>("topic", string(argv[2])));
		default:
			_clientRequest.cmdCode = HELP;
		}
	}

	if (!strncmp("upload", argv[1], 6))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.cmdCode = UPLOAD;
			_clientRequest.requestData.insert( pair<string,string>("file_name", string(argv[2])));
			break;
		default:
			_clientRequest.cmdCode = HELP;
			_clientRequest.requestData.insert( pair<string,string>("topic", string(argv[1])));
		}
	}

	if (!strncmp("download", argv[1], 8))
	{
		switch (argc)
		{
		case 4:
			_clientRequest.requestData.insert( pair<string,string>("download_path", string(argv[3])));
		case 3:
			_clientRequest.cmdCode = DOWNLOAD;
			_clientRequest.requestData.insert( pair<string,string>("file_name", string(argv[2])));
			break;
		default:
			_clientRequest.cmdCode = HELP;
			_clientRequest.requestData.insert( pair<string,string>("topic", string(argv[1])));
		}
	}

	if (!strncmp("delete", argv[1], 6))
	{
		switch (argc)
		{
		case 3:
			_clientRequest.cmdCode = DELETE;
			_clientRequest.requestData.insert( pair<string,string>("file_name", string(argv[2])));
			break;
		default:
			_clientRequest.cmdCode = HELP;
			_clientRequest.requestData.insert( pair<string,string>("topic", string(argv[1])));
		}
	}

	if (!strncmp("list", argv[1], 4))
	{
		switch (argc)
		{
		case 2:
			_clientRequest.cmdCode = LIST;
		case 3:
			_clientRequest.requestData.insert( pair<string,string>("path", string(argv[2])));
			break;
		default:
			_clientRequest.cmdCode = HELP;
			_clientRequest.requestData.insert( pair<string,string>("topic", string(argv[1])));
		}
	}

	if (!strncmp("register", argv[1], 8))
	{
		_clientRequest.cmdCode = REGISTER;
	}

	if (!strncmp("login", argv[1], 5))
	{
		_clientRequest.cmdCode = LOGIN;
	}
}

int ClientApp::ExecuteRequest()
{
	switch (_clientRequest.cmdCode)
	{
	case HELP:
		if(_clientRequest.requestData.count("topic"))
			return Help(_clientRequest.requestData.at("topic"));
		else
			return Help();
	case UPLOAD:
		return UploadFile(_clientRequest.requestData.at("file_name"));
	case DOWNLOAD:
		if(_clientRequest.requestData.count("download_path"))
			return DownloadFile(_clientRequest.requestData.at("file_name"),
				_clientRequest.requestData.at("download_path"));
		else
			return DownloadFile(_clientRequest.requestData.at("file_name"));
	case DELETE:
		return DeleteFile(_clientRequest.requestData.at("file_name"));
	case LIST:
		if(_clientRequest.requestData.count("path"))
			return ListDirectory(_clientRequest.requestData.at("path"));
		else
			return ListAll();
	case REGISTER:
		return RegisterUser();
	case LOGIN:
		return LoginUser();
	default:
		return -1;
	}
}

int ClientApp::UploadFile(string file_name)
{
	cout << "Upload file: " << file_name << "\n";
	return 0;
}

int ClientApp::DownloadFile(string file_name)
{
	cout << "Download file: " << file_name << " in current directory\n";
	return 0;
}

int ClientApp::DownloadFile(string file_name, string download_path)
{
	cout << "Download file: " << file_name << " in " << download_path << " directory\n";
	return 0;
}

int ClientApp::DeleteFile(string file_name)
{
	cout << "Delete file: " << file_name << "\n";
	return 0;
}

int ClientApp::ListAll()
{
	cout << "List directories recursively\n";
	return 0;
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

void CalculateHash(FILE* file)
{

}